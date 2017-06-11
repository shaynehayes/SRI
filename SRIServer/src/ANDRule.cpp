
/** 
	ANDRule.cpp
*/

#include "stdlib.h"
#include "iostream"
#include "ANDRule.h"
#include "ANDThread.h"
#include "ThreadManager.h"
#include "common.h"

using namespace std;

// ANDRule constructor
ANDRule::ANDRule(string name, vector<Statement*> operand, vector<vector<int> > params):Rule(name, operand, params){}
	
// ANDRule destructor
ANDRule::~ANDRule(){}

// getOperator()
// Returns the operator of this rule
string ANDRule::getOperator(){
	return "AND";
}

// queryStep()
// Steps the query forward one operand for one group
void ANDRule::queryStep(Group * g, vector<Group*> prevResults, vector<Group*> * out){
	if(prevResults.size() == operand.size()){ // The last operand has returned results
		Group * result = g->copy();
		vector<Group*> results;
		// Build the resulting Group from prevResults
		for(int j = prevResults.size()-1; j >= 0; j--){
			result->addParams(prevResults[j], params[j+1], params[0]);
		}
		// Enforce that the group adheres to the parameter filters
		// (So Rule($X,$X) will return only (x,x) pairs)
		bool followsRules = true;
		for(unsigned int i = 0; i < g->getSize(); i++){
			if(g->get(i)->getName()[0] == '$'){ // g->get(i) is a parameter
				for(unsigned int j = i; j < g->getSize(); j++){
					if(g->get(i) == g->get(j) && result->get(i) != result->get(j)){
						followsRules = false;
					}
				}
			}
		}
		// Add the result to out if it follows the parameter rules
		if(followsRules && result->numParams() == 0) (*out).push_back(result);
		else delete(result);
	}
	else{
		Group * sub;
		vector<Group*> temp;
		
		// Perform input parameter substitution
		sub = g->reorder(params[0], params[prevResults.size()+1]);
		// Perform parameter substitution of all prior operands
		for(int i = prevResults.size()-1; i >= 0; i--){
			sub->addParams(prevResults[i], params[i+1], params[prevResults.size()+1]);
		}
		// Query the operand with the new group
		temp = operand[prevResults.size()]->query(sub);
		delete(sub); // Delete sub
		vector<vector<Group*> > threadOut(temp.size(), vector<Group*>()); // Create a separate output vector for each group/thread
		// Create a new ThreadManager
		ThreadManager * threadManager = new ThreadManager();
		// Create a new ANDThread(this, prevResults, g, out) for each group
		for(unsigned int i = 0; i < temp.size(); i++){
			prevResults.push_back(temp[i]);
			threadManager->addThread(new ANDThread(this, prevResults, g, &threadOut[i]));
			prevResults.pop_back();
		}
		// Start the threads
		threadManager->start();
		// Block on barrier until all threads are finished
		threadManager->barrier();
		// Delete the ThreadManager
		delete(threadManager);
		// Delete temporary groups
		for(unsigned int i = 0; i < temp.size(); i++){
			if(temp[i] != NULL) delete(temp[i]);
		}
		// Compress and remove all duplicates from out
		(*out) = compressVector(threadOut);
	}
}

// query()
// Returns a vector of Groups that fill in the parameter
// blanks of Group g (based on this ANDRule)
vector<Group*> ANDRule::query(Group * g){
	vector<Group*> out;
	// Give queryStep the out vector so it can fill it in with valid groups
	queryStep(g, out, &out);
	// Return out
	return out;
}