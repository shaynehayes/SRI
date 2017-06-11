/** 
	Group.cpp
*/

#include "stdlib.h"
#include "iostream"
#include "Group.h"

using namespace std;

// Group constructor
Group::Group(){
	temp = true;
}

// Group destructor
Group::~Group(){
	for(unsigned int i = 0; i < member.size(); i++){
		if(member[i] != NULL && member[i]->getName() == "$*"){ // If the name is a temp parameter
			if(member[i]->getTemp() < 2) delete(member[i]); // Delete it if this is the last pointer to it
			else member[i]->addTemp(-1); // Else decrease the pointer count
		}
	}
}

// add()
// Adds a name to the end of this group
void Group::add(Name * name){
	member.push_back(name);
}

// fill()
// Replaces a parameter entry with a name
void Group::fill(unsigned int i, Name * name){
	if(i >= 0 && i < member.size() && member[i]->getName()[0] == '$') member[i] = name;
}

// setTemp()
// Sets the temp status of this group
// Groups marked as temp are assumed to be copies of
// fact groups and are thus freed during queries
// (Temp is true by default)
void Group::setTemp(bool value){
	temp = value;
}

// get()
// Fetches the name at index i of this group
Name* Group::get(unsigned int i){
	if(i < 0 || i >= member.size()) return NULL;
	return member[i];
}

// getSize()
// Returns the size of this group
unsigned int Group::getSize(){
	return member.size();
}

// numParams()
// Returns the number of parameter entries in this group
unsigned int Group::numParams(){
	int num = 0;
	for(unsigned int i = 0; i < member.size(); i++){
		if(member[i]->getName()[0] == '$') num++;
	}
	return num;
}

// getTemp()
// Returns the temp status of this group
bool Group::getTemp(){
	return temp;
}

// matches()
// Returns true if this group fills in the parameter
// blanks of g
bool Group::matches(Group * g){
	if(g->getSize() != member.size()) return false;
	for(unsigned int i = 0; i < member.size(); i++){
		// For the group to match, each non-parameter member in g must match its equivalent member in this group
		if(g->get(i)->getName()[0] != '$'){ // g->get(i) is not a parameter
			if(member[i] != g->get(i)) return false;
		}
		// Additionally, if two parameters in g are the same object, the equivalent names in this group must be the same
		else{ // g->get(i) is a parameter
			for(unsigned int j = i; j < member.size(); j++){
				if(g->get(i) == g->get(j) && member[i] != member[j]){
					return false;
				}
			}
		}
	}
	return true;
}

// equals()
// Returns true if Group g is (strictly) equal to this group
bool Group::equals(Group * g){
	if(g->getSize() != member.size()) return false;
	for(unsigned int i = 0; i < member.size(); i++){
		if(member[i] != g->get(i)) return false;
	}
	return true;
}

// reorder()
// Returns a new group that contains the members of this
// group, but reordered according to vectors order1 and order2
Group* Group::reorder(vector<int> order1, vector<int> order2){
	Group * g = new Group();
	for(unsigned int i = 0; i < order2.size(); i++){
		bool foundNum = false;
		for(unsigned int j = 0; (j < order1.size() && j < member.size()); j++){
			if(order1[j] == order2[i]){
				g->add(member[j]); // Adding a parameter that exists in both order1 and order2
				if(member[j]->getName() == "$*") member[j]->addTemp(1); // Tell the temp parameter that there is another pointer to it
				foundNum = true;
			}
		}
		if(!foundNum){ // Adding a new temp parameter that did not exist before
			bool foundParam = false;
			//First check to see if this temp parameter is already in the group
			for(int j = i-1; j >= 0; j--){
				if(order2[i] == order2[j]){
					// If it is, add it again
					g->add(g->get(j));
					g->get(j)->addTemp(1); // Tell the temp parameter that there is another pointer to it
					foundParam = true;
					break;
				}
			}
			// If it is not, create a new temp parameter
			if(!foundParam){
				Name * n = new Name("$*");
				n->addTemp(1); // Set temp to 1 to notify the destructor that this is a temp parameter
				g->add(n);
			}
		}
	}
	return g;
}

// addParams()
// Fills in parameter entries with names from g according to vectors
// order1 and order2
void Group::addParams(Group * g, vector<int> order1, vector<int> order2){
	for(unsigned int i = 0; i < order2.size(); i++){
		if(member[i]->getName()[0] == '$'){ // Member[i] is a parameter
			for(unsigned int j = 0; j < order1.size(); j++){
				if(order1[j] == order2[i]){
					if(member[i]->getName() == "$*"){ // If overwriting a temporary parameter, reduce temp or delete it
						member[i]->addTemp(-1);
						if(member[i]->getTemp() < 2) delete(member[i]);
					}
					member[i] = g->get(j);
					if(member[i]->getName() == "$*") member[i]->addTemp(1); // Tell the temp parameter that there is another pointer to it
				}
			}
		}
	}
}

// copy()
// Returns a new group that is a copy of this group
Group* Group::copy(){
	Group * g = new Group();
	for(unsigned int i = 0; i < member.size(); i++){
		g->add(member[i]);
		if(member[i]->getName() == "$*") member[i]->addTemp(1); // Tell the temp parameter that there is another pointer to it
	}
	return g;
}