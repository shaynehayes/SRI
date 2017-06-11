/** 
	Group.h
*/

#ifndef GROUP_H
#define GROUP_H

#include <vector>
#include "Name.h"

class Group
{
    public:
		// Group constructor
        Group();
		
		// Group destructor
        ~Group();
		
		// add()
		// Adds a name to the end of this group
		void add(Name * name);
		
		// fill()
		// Replaces a parameter entry with a name
		void fill(unsigned int i, Name * name);
		
		// setTemp()
		// Sets the temp status of this group
		// Groups marked as temp are assumed to be copies of
		// fact groups and are thus freed during queries
		// (Temp is true by default)
		void setTemp(bool value);
		
		// get()
		// Fetches the name at index i of this group
		Name* get(unsigned int i);
		
		// getSize()
		// Returns the size of this group
		unsigned int getSize();
		
		// numParams()
		// Returns the number of parameter entries in this group
		unsigned int numParams();
		
		// getTemp()
		// Returns the temp status of this group
		bool getTemp();
		
		// matches()
		// Returns true if this group fills in the parameter
		// blanks of g
		bool matches(Group * g);
		
		// equals()
		// Returns true if Group g is (strictly) equal to this group
		bool equals(Group * g);
		
		// reorder()
		// Returns a new group that contains the members of this
		// group, but reordered according to vectors order1 and order2
		Group* reorder(std::vector<int> order1, std::vector<int> order2);
		
		// addParams()
		// Fills in parameter entries with names according to vectors
		// order1 and order2
		void addParams(Group * g, std::vector<int> order1, std::vector<int> order2);
		
		// copy()
		// Returns a new group that is a copy of this group
		Group* copy();
    
	protected:
    private:
		bool temp;
		std::vector<Name*> member; // The name members of this group
};

#endif // GROUP_H