
/** 
	Name.h
*/

#ifndef NAME_H
#define NAME_H

#include <string>

class Name
{
    public:
		// Name constructor
        Name(std::string name);
		
		// Name destructor
        ~Name();
		
		// getName()
		// Returns string name
		std::string getName();
		
		// setName()
		// Sets string name
		void setName(std::string name);
		
		// addTemp()
		// Adds i to temp
		void addTemp(int i);

		// getTemp()
		int getTemp();
		
    protected:
    private:
		std::string name;
		int temp; // Used for temporary parameter cleanup (a last minute solution to fix a number of memory leaks)
};

#endif // NAME_H
