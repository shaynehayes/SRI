#ifndef ORTHREAD_H
#define ORTHREAD_H

#include "Thread.h"
#include "Statement.h"
#include "Group.h"

class ORThread : public Thread
{
	private:
		Statement * operand;
		std::vector<int> inparams;
		std::vector<int> opparams;
		Group * g;
		std::vector<Group*> * out;
	public:
		ORThread(Statement * operand, std::vector<int> inparams, std::vector<int> opparams, Group * g, std::vector<Group*> * out); // Constructor
		~ORThread(); // Destructor
		virtual void * threadMainBody (void * arg);
};

#endif // ORTHREAD_H