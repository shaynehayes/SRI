
/** 
	ANDRule.h
*/

#ifndef ANDRULE_H
#define ANDRULE_H

#include "Rule.h"

class ANDRule : public Rule
{
    public:
		// ANDRule constructor
        ANDRule(std::string name, std::vector<Statement*> operand, std::vector<std::vector<int> > params);
        
		// ANDRule destructor
		virtual ~ANDRule();
		
		// getOperator()
		// Returns the operator of this rule
		virtual std::string getOperator();
			
		// queryStep()
		// Steps the query forward one operand
		void queryStep(Group * g, std::vector<Group*> prevResults, std::vector<Group*> * out);
		
		// query()
		// Returns a vector of Groups that fill in the parameter
		// blanks of Group g (based on this ANDRule)
		virtual std::vector<Group*> query(Group * g);
		
    protected:
    private:
};

#endif // ANDRULE_H
