/*
	Fact.h
*/

#ifndef FACT_H
#define FACT_H

#include "Statement.h"

class Fact: public Statement
{
	public:
		// Fact constructor
		Fact(std::string name);

		// Fact destructor
		virtual ~Fact();

		// add()
		// Adds a group to this fact
		// Returns false if this fact already exists
		bool add(Group * g);
			
		// getGroups()
		// Returns all of the groups stored under this fact
		std::vector<Group*> getGroups();
		
		// dropGroups()
		// Drops all groups that fill in the parameter blanks of Group g
		bool dropGroups(Group * g);
		
		// empty()
		// Returns whether or not this Fact is empty
		virtual int empty();

		// query()
		// Returns a vector of Groups that fill in the param blanks of Group g
		virtual std::vector<Group*> query(Group * g);

	protected:
	private:
		std::vector<Group*> group;
};

#endif // FACT_H