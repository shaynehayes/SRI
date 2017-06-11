
/** 
	Rule.h
*/

#ifndef RULE_H
#define RULE_H

#include "Statement.h"

class Rule : public Statement
{
    public:
		// Rule constructor
        Rule(std::string name, std::vector<Statement*> operand, std::vector<std::vector<int> > params);
        
		// Rule destructor
		virtual ~Rule();
		
		// numParams()
		// Returns the number of parameters to this rule
		unsigned int numParams();
		
		// getOperator()
		// Returns the operator of this rule
		virtual std::string getOperator();
	
		// getOperands()
		// Returns the operands of this rule
		virtual std::vector<Statement*> getOperands();
		
		// empty()
		// Returns whether or not this Rule is empty
		virtual int empty();
		
		// dropOperand()
		// Drops dependencies on a certain fact or rule (for deleting facts/rules)
		virtual bool dropOperand(Statement * op);
		
		// getParams()
		// Returns the parameter substitution rules of this rule
		std::vector<std::vector<int> > getParams();
		
		// query()
		// Returns a vector of Groups that fill in the parameter
		// blanks of Group g (based on this rule)
		virtual std::vector<Group*> query(Group * g);
		
    protected:
		std::vector<Statement*> operand;
		std::vector<std::vector<int> > params;
    private:
};

#endif // RULE_H
