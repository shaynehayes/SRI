
/** 
	Session.h
*/

#ifndef SESSION_H
#define SESSION_H

#include <string>
#include <vector>
#include "Fact.h"
#include "Rule.h"
#include "Name.h"
#include "Group.h"
#include "ORRule.h"
#include "ANDRule.h"

class Session
{
    public:
		// Session constructor
        Session();
		
		// Session destructor
        ~Session();
		
		// split()
		// Splits a string at every non-alphanumerical
		std::vector<std::string> split(std::string str);
		
		// processCommand()
		// Translates user-entered string commands into SRI
		// instructions. Returns a string that contains the
		// output of SRI
		std::string processCommand(std::string command);

		// processRule()
		// Adds a new OR or AND rule in the Rule Base
		std::string processRule(std::vector<std::string> vect);

		// processFact()
		// Adds a new fact to the Knowledge Base
		std::string processFact(std::vector<std::string> vect);

		// processInference()
		std::string processInference(std::vector<std::string> vect);

		// processDrop()
		// Deletes every fact and rule with the specified name
		std::string processDrop(std::vector<std::string> vect);

		// sessionDump()
		std::string sessionDump();
		
    protected:
    private:
		std::vector<Name*> name;
		std::vector<Fact*> KB;
		std::vector<Rule*> RB;
};

#endif // SESSION_H
