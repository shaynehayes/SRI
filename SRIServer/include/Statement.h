
/** 
	Statement.h
*/

#ifndef STATEMENT_H
#define STATEMENT_H

#include <string>
#include <vector>
#include "Group.h"
#include "Name.h"

class Statement
{
    public:
		// Statement constructor
        Statement(std::string name);
		
		// Statement destructor
        virtual ~Statement();
		
		// dropOperand()
		// Drops dependencies on a certain statement (for deleting statements)
		virtual bool dropOperand(Statement * op);
		
		// empty()
		// Returns whether or not this Statement is empty
		virtual int empty();
		
		// getName()
		// Returns this Statement's name string
		std::string getName();
		
		// query()
		// Returns a vector of Groups that fill in the parameter
		// blanks of Group g (based on this statement)
		virtual std::vector<Group*> query(Group * g);
		
    protected:
		std::string name;
    private:
};

#endif // STATEMENT_H
