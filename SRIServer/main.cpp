
/** 
	main.cpp
*/

#include <string>
#include "stdlib.h"
#include "iostream"
#include <fstream>

#include "GarbageCollector.h"
#include "TCPServerSocket.h"

using namespace std;

int main(int argc, char *argv[]){
	// Instantiate a new TCPSocketServer Object that listens on all interfaces on port 9999
	TCPServerSocket * tcpServerSocket = new TCPServerSocket("0.0.0.0",9999,100);
	// Instantiate a connection garbage collector
	GarbageCollector * garbageCollector = new GarbageCollector();
	// Initialize the server socket
	tcpServerSocket->initializeSocket();
	cout << "[Welcome to SRI!] Waiting for new connections..." << endl;
	for ( ;; ) // Loop forever
	{
		// Wait for connection and return a TCPSocket object upon one
		TCPSocket * tcpSocket = tcpServerSocket->getConnection();
		if (tcpSocket == NULL) break; // if tcpSocket is NULL then error occured and we break the loop
		garbageCollector->cleanup(); // Invoke the garbage collector cleaner in case of expired connections
		Connection * c = new Connection (tcpSocket); // Instantiate a new Connection object and pass the returned TCP socket to it
		c->start(); // Start the connection thread to communicate with the client
		garbageCollector->addConnection(c); // Add the connection to the garbage collector to be cleaned up when expired
	}
	delete(garbageCollector); // Delete the garbage collector
	delete (tcpServerSocket); // Terminate and delete the TCP server socket
	return 0;
}