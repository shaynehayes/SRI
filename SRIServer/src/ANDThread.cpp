#include "ANDThread.h"

ANDThread::ANDThread(ANDRule * thisRule, vector<Group*> prevResults, Group * g, vector<Group*> * out){
	this->thisRule = thisRule;
	this->prevResults = prevResults;
	this->g = g;
	this->out = out;
}
ANDThread::~ANDThread(){}
void * ANDThread::threadMainBody(void * arg){
	// Start the query for the next operand in a new thread
	thisRule->queryStep(g, prevResults, out);
	return NULL;
}