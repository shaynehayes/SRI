
/** 
	ORRule.h
*/

#ifndef ORRULE_H
#define ORRULE_H

#include "Rule.h"

class ORRule : public Rule
{
    public:
		// ORRule constructor
        ORRule(std::string name, std::vector<Statement*> operand, std::vector<std::vector<int> > params);
        
		// ORRule destructor
		virtual ~ORRule();
		
		// getOperator()
		// Returns the operator of this rule
		virtual std::string getOperator();
		
		// query()
		// Returns a vector of Groups that fill in the parameter
		// blanks of Group g (based on this ORRule)
		virtual std::vector<Group*> query(Group * g);
		
    protected:
    private:
};

#endif // ORRULE_H
