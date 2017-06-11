#include "ThreadManager.h"

// Constructor
ThreadManager::ThreadManager() {}

// Add a thread
void ThreadManager::addThread (Thread * p_thread) {
	threads.push_back(p_thread); // Push back thread pointer to threads vector
}

// Start all threads
void ThreadManager::start() {
	// Loop on all vector threads and start them
	for (unsigned int i = 0 ; i < threads.size();i++) threads[i]->start();
}

// Wait on barrier on all threads
void ThreadManager::barrier() {
	// Loop on all vector thread and block until terminated
	for (unsigned int i = 0 ; i < threads.size();i++) threads[i]->waitForRunToFinish();
}

// Destructor
ThreadManager::~ThreadManager() {
	// Loop on threads vector and delete all threads
	for (unsigned int i = 0 ; i < threads.size();i++) delete (threads[i]);
}