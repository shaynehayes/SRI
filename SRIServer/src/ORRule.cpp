
/** 
	ORRule.cpp
*/

#include "stdlib.h"
#include "iostream"
#include "ORRule.h"
#include "ORThread.h"
#include "ThreadManager.h"
#include "common.h"

using namespace std;

// ORRule constructor
ORRule::ORRule(string name, vector<Statement*> operand, vector<vector<int> > params):Rule(name, operand, params){
	bool containsIn;
	bool foundParam;
	// params[0] is always the input parameters
	vector<int> in = this->params[0];
	vector<int> temp;
	// Enforce that all operands contain the input parameters
	// For instance, if the rule is Name($X,$Y,$Z) :- Name2($X,$Y,$Z) Name3($Y,$Z) Name4($X,$Y,$Z,$A),
	// Name3 will be dropped from the operands list along with its parameters
	for(unsigned int i = 1; i < this->params.size(); i++){
		temp = this->params[i];
		containsIn = true;
		for(unsigned int j = 0; j < in.size(); j++){
			foundParam = false;
			for(unsigned int r = 0; r < temp.size(); r++){
				if(temp[r] == in[j]){foundParam = true; break;}
			}
			if(!foundParam){containsIn = false; break;}
		}
		// If the operand does not include all of the input parameters, remove it (it will never be executed)
		if(!containsIn){
			this->operand.erase(this->operand.begin() + (i-1)); // The ith parameter list corresponds to the i-1th operand
			this->params.erase(this->params.begin() + i);
			i--; // Step back one iteration
		}
	}
}
	
// ORRule destructor
ORRule::~ORRule(){}

// getOperator()
// Returns the operator of this rule
string ORRule::getOperator(){
	return "OR";
}

// query()
// Returns a vector of Groups that fill in the parameter
// blanks of Group g (based on this ORRule)
vector<Group*> ORRule::query(Group * g){
	vector<Group*> out;
	vector<vector<Group*> > threadOut(operand.size(), vector<Group*>()); // Create a separate output vector for each operand/thread
	// Create a new ThreadManager
	ThreadManager * threadManager = new ThreadManager();
	// Create a new ORThread(operand[i], params[0], params[i+1], g, &out) for each operand
	for(unsigned int i = 0; i < operand.size(); i++){
		threadManager->addThread(new ORThread(operand[i], params[0], params[i+1], g, &threadOut[i]));
	}
	// Start the threads
	threadManager->start();
	// Block on barrier until all threads are finished
	threadManager->barrier();
	// Delete the ThreadManager
	delete(threadManager);
	// Compress and remove all duplicates from out
	out = compressVector(threadOut);
	// Return out
	return out;
}
