
/** 
	Session.cpp
*/

#include "stdlib.h"
#include "iostream"
#include <sstream>
#include "common.h"
#include "Session.h"
#include "SessionThread.h"
#include "ThreadManager.h"

using namespace std;

// Session constructor
Session::Session(){}

// Session destructor
Session::~Session(){
	for(unsigned int i = 0; i < RB.size(); i++){
		delete(RB[i]);
	}
	for(unsigned int i = 0; i < KB.size(); i++){
		delete(KB[i]);
	}
	for(unsigned int i = 0; i < name.size(); i++){
		delete(name[i]);
	}
}

// split()
// Splits a string at every non-alphanumerical, excluding $
vector<string> Session::split(string str){
    vector<string> vect;
    stringstream ss(str); //Turn string into a stream
    string token;
    char c;

    for(unsigned int i = 0; i < str.length(); i++) {
        c = str[i];
        //if c is not alphanumeric
        if(!isalpha((int) c) && !isdigit(c) && c != '$') {
            /*
             * the code below prints the entirety of the string
             * so getline is doing something unique to keep track of
             * where ss was split by the delimiter
             */
            while(getline(ss,token,c)) {
                if(!token.empty()) {
                    vect.push_back(token);
                }
                break;
            }
        }
    }
    getline(ss,token,'\n');
    if(!token.empty()) vect.push_back(token);
    return vect;
}

// processRule()
// Adds a new OR or AND rule in the Rule Base
string Session::processRule(vector<string> vect){
	vector<string> pass,var; // Used to pass strings of names and operands and var stores $_'s
    vector<int> tempParam; // Temporarily hold parameters of a name ($X, $Y, $Z):(0, 1, 2)
    vector<vector<int> > param; // Used to pass parameter indexes
    bool passing = false;
	
	// Replace parameters with indexes; can't be threaded because order matters
    for(unsigned int i = 0; i < vect.size(); i++){
    	if(vect[i][0] == '$'){ // Current string is a parameter

  			// Check if parameter is stored in var
	        passing = true;
	        bool equal = false;
	        int p = 0;
	        for(; p < (signed)var.size(); p++){
	            if(!var[p].compare(vect[i])){
	                equal = true;
	                break;
	            }
	        }

        	// Index was found
	        if(equal) tempParam.push_back(p);
	        // Index was not found
	        else{
	            var.push_back(vect[i]);
	            tempParam.push_back(p);
	        }
	    }
		else{ // Current string is not a parameter
	        if(passing) {
	            param.push_back(tempParam);
	            tempParam.clear();
	            passing = false;
	        }
	        pass.push_back(vect[i]);
	    }
    }
    // for loop breaks before pushing the last batch of parameters
    if(!tempParam.empty()) {
        param.push_back(tempParam);
        tempParam.clear();
    }

	vector<Statement*> op(pass.size()-3, NULL);
	bool isOR;
	// Catch any syntax errors
	// pass[0] = RULE, pass[1] = rule name, pass[2] = operator
	if(pass.size() < 3) return "Usage: RULE Name($Param1,$Param2,...):- OR/AND Predicate1($Param1,$Param2,...) ...\n";
	else if(pass.size()-3 != param.size()-1) return "[Error] Each predicate must have at least one parameter!\n";
	if (!pass[2].compare("OR")) isOR = true;
	else if (!pass[2].compare("AND")) isOR = false;
	else return "[Error] Operator " + pass[2] + " is not a valid logical operator!\n";
	
	// Fetch all operand pointers in parallel
	string msg = "";
	// Create a new ThreadManager
	ThreadManager * threadManager = new ThreadManager();
	// Create a new SessionThread(pass[i], KB, RB, param[i], &msg, &operand) for each operand
	for(unsigned int i = 3; i < pass.size(); i++){
		threadManager->addThread(new SessionThread(KB, RB, pass[i], param[i-2], &msg, &op[i-3]));
	}
	// Start the threads
	threadManager->start();
	// Block on barrier until all threads are finished
	threadManager->barrier();
	// Delete the ThreadManager
	delete(threadManager);
	
	// Catch any syntax errors
	if(!msg.empty()) return msg;
	for(unsigned int i = 0; i < op.size(); i++) if(op[i] == NULL) return "[Error] This rule must have at least one valid predicate!\n";
	
	// Pass parameters to operator
	Rule * newRule;
	if(isOR) newRule = new ORRule(pass[1],op,param);
	else newRule = new ANDRule(pass[1],op,param);
	
	// Delete the rule if all of its predicates are invalid, otherwise add it to the Rule Base!
	if(newRule->getOperands().size() == 0){
		delete(newRule);
		return "[Error] This rule must have at least one valid predicate!\n";
	}
	else RB.push_back(newRule);

	return "Rule " + vect[1] + " added successfully!\n";
}

// processFact()
// Adds a new fact to the Knowledge Base
string Session::processFact(vector<string> vect){
	
	if(vect.size() < 3) return "Usage: FACT Name(Name1,Name2,...)\n";
	
	// Loop through Knowledge Base
	// Determine whether or not the fact already exists
	bool inKB = false;
	Fact * fact;
	for(unsigned int a = 0; a < KB.size(); a++){
		if(!vect[1].compare(KB[a]->getName())){inKB = true; fact = KB[a]; break;}
	}
	
	// Make the fact if it doesn't exist
	if(!inKB){
		fact = new Fact(vect[1]);
		KB.push_back(fact);
	}
	
	// Create a new Group
	Group * group = new Group();

	// Loop through the rest of the vector for names
	for(unsigned int b = 2; b < vect.size(); b++){
		Name * nameT;
		bool inName = false;
		// Determine whether or not the name already exists
		for(unsigned int c = 0; c < name.size(); c++) {
			if(!vect[b].compare(name[c]->getName())) {inName = true; nameT = name[c]; break;}
		}

		// Name does not exist in name so create new name
		if(!inName) {nameT = new Name(vect[b]); name.push_back(nameT);}
		// Add the name into the new group
		group->add(nameT);
	}

	// Finally attempt to add the new group of names into the fact
	bool addIntoKB = fact->add(group);
	if(addIntoKB) return "Fact " + vect[1] + " added successfully!\n";
	else{delete(group); return "[Error] Fact " + vect[1] + " already exists in the Knowledge Base!\n";}
}

// processInference()
// Makes an inference based on the Knowledge Base and Rule Base
string Session::processInference(vector<string> vect) {
	
	if(vect.size() < 4) return "Usage: INFERENCE Name(Name1/$Param1,Name2/$Param2,...) SaveName(optional)\n";
	
	string out = "";
	
	// Create a new Group
	Group * group = new Group();
	
	// Loop through the rest of the vector for names
	for(unsigned int b = 2; b < vect.size()-1; b++) {
		bool inName = false;
		Name * nameT;

		// Loop through names to find if names already exists
		for(unsigned int c = 0; c < name.size(); c++){
			if(!vect[b].compare(name[c]->getName())){inName = true; nameT = name[c]; break;}
		}

		// Name does not exist in name so create new name
		if(!inName) {nameT = new Name(vect[b]); name.push_back(nameT);}
		// Add the name into the new group
		group->add(nameT);
	}
	
	// Call the query function of each Fact and Rule with this name, concatenating the results
	vector<Group*> temp;
	vector<Group*> results;
	for(unsigned int i = 0; i < KB.size(); i++){
		if(!vect[1].compare(KB[i]->getName())){
			temp = KB[i]->query(group);
			// For each returned group...
			for(unsigned int j = 0; j < temp.size(); j++){
				if(!isDuplicate(temp[j], results)) results.push_back(temp[j]); // If it is not a duplicate, add it to results
				else delete(temp[j]); // If it is a duplicate, delete it and forget about it
			}
		}
	}
	for(unsigned int i = 0; i < RB.size(); i++){
		if(!vect[1].compare(RB[i]->getName()) && RB[i]->getParams()[0].size() == group->getSize()){
			temp = RB[i]->query(group);
			// For each returned group...
			for(unsigned int j = 0; j < temp.size(); j++){
				if(!isDuplicate(temp[j], results)) results.push_back(temp[j]); // If it is not a duplicate, add it to results
				else delete(temp[j]); // If it is a duplicate, delete it and forget about it
			}
		}
	}
	
	// Print out the results or save them as a fact
	// Printing results
	if(!vect[vect.size()-1].compare("")){
		out = out + "Inference Results:\n";
		// Loop through the results and output their names, deleting as we go
		Name * n;
		int printed = 0; // Keeps track of how many variables have been printed
		bool printedResult = false; // Keeps track of how many results have been printed
		for(unsigned int i = 0; i < results.size(); i++){
			printed = 0;
			for(unsigned int j = 0; j < results[i]->getSize(); j++){
				n = results[i]->get(j);
				if(vect[j+2][0] == '$'){
					bool alreadyPrinted = false; // Make sure this variable result hasn't already been printed
					for(int p = j+1; p > 1; p--) if(vect[j+2] == vect[p]) alreadyPrinted = true;
					if(!alreadyPrinted){
						if(printed != 0) out = out + ", ";
						out = out + vect[j+2] + ":";
						if(n != NULL) out = out + n->getName();
						printed++;
						printedResult = true;
					}
				}
			}
			out = out + "\n";
			delete(results[i]);
			if(i == results.size()-1 && !printedResult){ // If there is a result but nothing has been printed, print "This relation exists!"
				out = out + "This relation exists!\n";
			}
		}
	}
	else{ // Saving results as a fact
	
		// Loop through Knowledge Base
		// Determine whether or not the fact already exists
		bool inKB = false;
		Fact * fact;
		for(unsigned int a = 0; a < KB.size(); a++){
			if(!vect[vect.size()-1].compare(KB[a]->getName())){inKB = true; fact = KB[a]; break;}
		}
			
		// Make the fact if it doesn't exist
		if(!inKB){
			fact = new Fact(vect[vect.size()-1]);
			KB.push_back(fact);
		}
		
		// Add the result groups to the fact
		for(unsigned int i = 0; i < results.size(); i++){
			fact->add(results[i]);
		}
	}
	
	delete(group);
	return out;
}

// processDrop
// Deletes every fact and rule with the specified name
string Session::processDrop(vector<string> vect) {
	
	if(vect.size() < 2) return "Usage: DROP Name(Name1/$Param1,Name2/$Param2,...)\n";
	
	// Create a new Group
	Group * group = new Group();
	
	// Loop through the rest of the vector for names
	for(unsigned int b = 2; b < vect.size(); b++){
		bool inName = false;
		Name * nameT;

		// Loop through names to find if names already exists
		for(unsigned int c = 0; c < name.size(); c++){
			if(!vect[b].compare(name[c]->getName())){inName = true; nameT = name[c]; break;}
		}

		// Name does not exist in name so create new name
		if(!inName) {nameT = new Name(vect[b]); name.push_back(nameT);}
		// Add the name into the new group
		group->add(nameT);
	}
	
	int num = 0;
		
	// Loop through Knowledge Base
	for(unsigned int a = 0; a < KB.size(); a++){
		if(!vect[1].compare(KB[a]->getName())){
			// If a matching fact is found, tell it to drop any facts that fill in the parameter blanks of group
			bool deleted = KB[a]->dropGroups(group);
			// Increment the deletion counter if a group was deleted
			if(deleted) num++;
		}
	}
	// Loop through Rule Base
	for(unsigned int a = 0; a < RB.size(); a++){
		if(!vect[1].compare(RB[a]->getName())){
			// Drop any predicates that depend on this rule
			for(unsigned int b = 0; b < RB.size(); b++) RB[b]->dropOperand(RB[a]);
			// Delete the rule
			delete(RB[a]);
			RB.erase(RB.begin() + a);
			a--;
			// Increment the deletion counter
			num++;
		}
	}
	// Delete any rules without operands
	for(unsigned int b = 0; b < RB.size(); b++){
		if(RB[b]->getOperands().size() == 0){
			delete(RB[b]);
			RB.erase(RB.begin() + b);
			b--;
		}
	}
	if(num == 0) return "[Error] There are no facts or rules of this name to drop!\n";
	return "Successfully dropped facts/rules from the Knowledge Base and Rule Base!\n";
}

string Session::sessionDump() {
	
	string out = "";
	
	// Knowledge Base DUMP
	// Loop through the Knowledge Base
	for(unsigned int a = 0; a < KB.size(); a++){
		
		// Loop through each group of the fact and add it to out
		vector<Group*> f = KB[a]->getGroups();
		for(unsigned int b = 0; b < f.size(); b++){
			out = out + "FACT " + KB[a]->getName() + "(";
			
			// Loop through the names in the group
			for(unsigned int c = 0; c < f[b]->getSize(); c++){
				Name* n = f[b]->get(c); // Margret,Bob,Mike
				out += n->getName();
				if(c != f[b]->getSize()-1) out += ",";
			}
			out = out + ")\n";
		}
	}
	
	// Rule Base DUMP
	// Loop through the Rule Base
	for(unsigned int a = 0; a < RB.size(); a++) {
		
		vector<Statement*> op = RB[a]->getOperands();
		vector<vector<int> > params = RB[a]->getParams();
		
		out = out + "RULE " + RB[a]->getName() + "(";
		// Add input parameters
		for(unsigned int i = 0; i < params[0].size(); i++){
			stringstream sparam; // Convert the param into a string
			sparam << params[0][i];
			out = out + "$" + sparam.str();
			if(i != params[0].size()-1) out = out + ",";
		}
		out = out + "):- " + RB[a]->getOperator();
		
		// Add the name of each operand and their parameters
		for(unsigned int b = 0; b < op.size(); b++){
			out = out + " " + op[b]->getName() + "(";
			// Add parameters
			for(unsigned int i = 0; i < params[b+1].size(); i++){
				stringstream sparam; // Convert the param into a string
				sparam << params[b+1][i];
				out = out + "$" + sparam.str();
				if(i != params[b+1].size()-1) out = out + ",";
			}
			out = out + ")";
		}
		out = out + "\n";
	}
	
	return out;
}

// processCommand()
// Translates user-entered string commands into SRI
// instructions. Returns a string that contains the
// output of SRI
string Session::processCommand(string command){
	vector<string> token = split(command);
	string out;

    if(!token[0].compare("FACT") || !token[0].compare("Fact") || !token[0].compare("fact")) out = processFact(token);
    else if(!token[0].compare("RULE") || !token[0].compare("Rule") || !token[0].compare("rule")) out = processRule(token);
    else if(!token[0].compare("INFERENCE") || !token[0].compare("Inference") || !token[0].compare("inference")){
		// Determine whether the inference should output to the terminal or save results under a fact
		for(int i = command.length()-1; i >= 0; i--){
			if(command[i] == ' ') continue; // Skip past white spaces at the end of the string
			else if(command[i] == ')'){ // If the first non-space char is ), we know we're in output mode
				token.push_back(""); // Add an empty token at the end so inference knows to output
				break;
			}
			else break; // Otherwise we're saving results under a fact
		}
		out = processInference(token);
	}
    else if(!token[0].compare("DROP") || !token[0].compare("Drop") || !token[0].compare("drop")) out = processDrop(token);
    else if(!token[0].compare("DUMP") || !token[0].compare("Dump") || !token[0].compare("dump")) out = sessionDump();
	else out = "[Error] Command " + token[0] + " not recognized.\n";
	
	return out;
}
