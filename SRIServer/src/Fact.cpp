/** 
	Fact.cpp
*/

#include "stdlib.h"
#include "iostream"
#include "Fact.h"

using namespace std;

// Fact constructor
Fact::Fact(string name):Statement(name){}

// Fact destructor
Fact::~Fact(){
	for(unsigned int i = 0; i < group.size(); i++){
		if(group[i] != NULL) delete(group[i]);
	}
}

// add()
// Adds a group to this fact
// Returns false if this fact already exists
bool Fact::add(Group * g){
	// Compare g to all of this fact's groups and return false if the same group is found
	for(unsigned int i = 0; i < group.size(); i++){
		if(g->equals(group[i])) return false;
	}
	// Otherwise, add g to the group vector
	group.push_back(g);
	// And set g's temp status so it is not deleted during queries
	g->setTemp(false);
	return true;
}

// getGroups()
// Returns all of the groups stored under this fact
std::vector<Group*> Fact::getGroups(){
	return group;
}

// dropGroups()
// Drops all groups that fill in the parameter blanks of Group g
bool Fact::dropGroups(Group * g){
	bool deleted = false;
	for(unsigned int i = 0; i < group.size(); i++){
		if(group[i]->matches(g) || g->getSize() == 0){ // Delete all groups if g is empty
			delete(group[i]);
			group.erase(group.begin() + i);
			i--;
			deleted = true;
		}
	}
	return deleted;
}

// empty()
// Returns whether or not this Fact is empty
int Fact::empty(){return (group.size() == 0);}

// query()
// Returns a vector of Groups that fill in the parameter
// blanks of Group g
vector<Group*> Fact::query(Group * g){
	vector<Group*> out;
	// Compare g to all of the groups in the group vector
	// If the entries of g match the corresponding entries of one of this fact's groups, add a copy of the group to vector out
	// For example, if g is {Ben, $X, Bert} and this fact contains a group {Ben, Amy, Bert}, a copy of this
	// group will be added to out
	for(unsigned int i = 0; i < group.size(); i++){
		if(group[i]->matches(g)){
			out.push_back(group[i]->copy());
		}
	}
	return out;
}