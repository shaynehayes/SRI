#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <vector>
#include "Thread.h"

// A thread manager class that manages a group of threads
class ThreadManager
{
	private:
		std::vector<Thread*> threads; // Vector of Thread pointers
	public:
		ThreadManager(); // Constructor
		~ThreadManager(); // Destructor
		void addThread (Thread * p_thread); // Add a thread
		void start(); // Start all threads
		void barrier(); // Wait for all threads to finish
};

#endif // THREADMANAGER_H_H