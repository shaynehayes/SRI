
/** 
	Rule.cpp
*/

#include "stdlib.h"
#include "iostream"
#include "Rule.h"

using namespace std;

// Rule constructor
Rule::Rule(string name, vector<Statement*> operand, vector<vector<int> > params):Statement(name){
	this->operand = operand;
	this->params = params;
}
	
// Rule destructor
Rule::~Rule(){}

// numParams()
// Returns the number of parameters to this rule
unsigned int Rule::numParams(){
	return params[0].size();
}

// getOperator()
// Returns the operator of this rule
string Rule::getOperator(){
	return "NULL";
}

// getOperands()
// Returns the operands of this rule
vector<Statement*> Rule::getOperands(){
	return operand;
}

// empty()
// Returns whether or not this Fact is empty
int Rule::empty(){return (operand.size() == 0);}

// dropOperand()
// Drops dependencies on a certain fact or rule (for deleting facts/rules)
bool Rule::dropOperand(Statement * op){
	bool droppedOperand = false;
	for(unsigned int i = 0; i < operand.size(); i++){
		// First tell this rule's operands to try to drop this operand
		// If all of an operand's operands are dropped, drop the operand (confused yet?)
		// Otherwise drop the operand if it is the operand being searched for
		operand[i]->dropOperand(op);
		if(operand[i]->empty() == true || operand[i] == op){ 
			operand.erase(this->operand.begin() + i); // Remove the operand and its parameters from this rule
			this->params.erase(this->params.begin() + (i+1));
			i--; // Step back one iteration
			droppedOperand = true;
		}
	}
	return droppedOperand;
}

// getParams()
// Returns the parameter substitution rules of this rule
vector<vector<int> > Rule::getParams(){
	return params;
}

// query()
// Returns a vector of Groups that fill in the parameter
// blanks of Group g (based on this rule)
vector<Group*> Rule::query(Group * g){
	vector<Group*> out;
	return out; // Return an empty vector
}