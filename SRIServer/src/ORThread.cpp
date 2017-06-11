#include "ORThread.h"

ORThread::ORThread(Statement * operand, vector<int> inparams, vector<int> opparams, Group * g, vector<Group*> * out){
	this->operand = operand;
	this->inparams = inparams;
	this->opparams = opparams;
	this->g = g;
	this->out = out;
}
ORThread::~ORThread(){}
void * ORThread::threadMainBody(void * arg){
	Group * sub;
	vector<Group*> temp;
	// Perform parameter substitution
	sub = g->reorder(inparams, opparams);
	// Query the operand with the reordered group
	temp = operand->query(sub);
	// Delete sub
	delete(sub);
	// For each returned group, undo parameter substitution and add to out
	for(unsigned int j = 0; j < temp.size(); j++){
		sub = temp[j]->reorder(opparams, inparams);
		if(temp[j]->getTemp()) delete(temp[j]); // Free temporary groups as we loop through them
		(*out).push_back(sub); // Add the group to out
	}
	return NULL;
}