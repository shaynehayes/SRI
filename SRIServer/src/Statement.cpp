
/** 
	Statement.cpp
*/

#include "stdlib.h"
#include "iostream"
#include "Statement.h"

using namespace std;

// Statement constructor
Statement::Statement(string name){
	this->name = name;
}

// Statement destructor
Statement::~Statement(){}

// dropOperand()
// Drops dependencies on a certain statement (for deleting statements)
bool Statement::dropOperand(Statement * op){return false;}

// empty()
// Returns whether or not this Statement is empty
int Statement::empty(){return true;}

// getName()
// Returns this Statement's name string
string Statement::getName(){
	return name;
}
// query()
// Returns a vector of Groups that fill in the parameter
// blanks of Group g (based on this statement)
vector<Group*> Statement::query(Group * g){
	vector<Group*> temp;
	return temp; // Return an empty vector
}