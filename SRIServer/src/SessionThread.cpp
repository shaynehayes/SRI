#include "SessionThread.h"

SessionThread::SessionThread(vector<Fact*> KB, vector<Rule*> RB, string pass, vector<int> param, string * msg, Statement ** op){
	this->KB = KB;
	this->RB = RB;
	this->pass = pass;
	this->param = param;
	this->msg = msg;
	this->op = op;
}
SessionThread::~SessionThread(){}

void * SessionThread::threadMainBody(void * arg){
	Statement * s;
	bool foundOp = false;
	// Find operand in KB
	for(unsigned int j = 0; j < KB.size(); j++){
		if(pass == KB[j]->getName()){s = KB[j]; foundOp = true; break;}
	}
	// Find operand in RB
	if(!foundOp){
		for(unsigned int j = 0; j < RB.size(); j++){
			if(pass == RB[j]->getName() && param.size() == RB[j]->numParams()){s = RB[j]; foundOp = true; break;}
		}
	}
	if(foundOp) (*op) = s;
	else *msg = "[Error] Predicate " + pass + " not found in the Knowledge Base or Rule Base!\n";
	return NULL;
}