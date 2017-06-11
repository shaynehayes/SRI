#ifndef ANDTHREAD_H
#define ANDTHREAD_H

#include "Thread.h"
#include "ANDRule.h"
#include "Group.h"

class ANDThread : public Thread
{
	private:
		ANDRule * thisRule;
		std::vector<Group*> prevResults;
		Group * g;
		std::vector<Group*> * out;
	public:
		ANDThread(ANDRule * thisRule, std::vector<Group*> prevResults, Group * g, std::vector<Group*> * out); // Constructor
		~ANDThread(); // Destructor
		virtual void * threadMainBody (void * arg);
};

#endif // ANDTHREAD_H