#ifndef SESSIONTHREAD_H
#define SESSIONTHREAD_H

#include "Thread.h"
#include "Session.h"

class SessionThread : public Thread
{
	private:
		std::vector<Fact*> KB;
		std::vector<Rule*> RB;
		std::string pass;
		std::vector<int> param;
		std::string * msg;
		Statement ** op;
	public:
		SessionThread(std::vector<Fact*> KB, std::vector<Rule*> RB, string pass, std::vector<int> param, std::string * msg, Statement ** op); // Constructor
		~SessionThread(); // Destructor
		virtual void * threadMainBody (void * arg);
};

#endif // SESSIONTHREAD_H