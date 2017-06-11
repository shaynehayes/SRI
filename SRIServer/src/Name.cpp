
/** 
	Name.cpp
*/

#include "stdlib.h"
#include "iostream"
#include "Name.h"

using namespace std;

// Name constructor
Name::Name(string name){
	this->name = name;
	temp = 0;
}
// Name destructor
Name::~Name(){}

// getName()
// Returns string name
string Name::getName(){
	return name;
}

// setName()
// Sets string name
void Name::setName(string name){
	this->name = name;
}

// addTemp()
// Adds i to temp
void Name::addTemp(int i){
	temp += i;
}

// getTemp()
int Name::getTemp(){
	return temp;
}