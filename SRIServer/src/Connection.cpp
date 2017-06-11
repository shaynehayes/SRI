
/** 
	Connection.cpp
*/

#include "Connection.h"
#include "Session.h"

//Constructor: Call parent Thread Constructor
Connection::Connection(TCPSocket * p_tcpSocket): Thread(){
	tcpSocket = p_tcpSocket; // Set the TCP socket
	next_connection = NULL; // Set the next pointer to NULL
}

// A modifier that sets the pointer of the next connection
void Connection::setNextConnection(Connection * connection){next_connection = connection;}

// A Selectot that returned a pointer to the next connection
Connection * Connection::getNextConnection (){return next_connection;}

// Destructor: delete the TCP socket if set
Connection::~Connection(){if ( tcpSocket != NULL ) delete (tcpSocket);}

// Main thread body for serving the connection
void * Connection::threadMainBody (void * arg) {
	cout << "Opened connection " << tcpSocket->getRemoteAddress() << endl;
	char command[1024]; // A buffer for holding commands from the client
	memset (command,0,1024); // Initialize the buffer
	Session * session = new Session(); // Create a new session
	for( ;; ){
		int read_bytes = tcpSocket->readFromSocket(command,1023); // Read from socket the command to be executed
		if ( read_bytes > 0) { // If read successfully
			// Clean up the command
			if ( command[strlen(command)-1]=='\n' || command[strlen(command)-1]== '\r') command[strlen(command)-1] = 0;
			if ( command[strlen(command)-1]=='\n' || command[strlen(command)-1]=='\r') command[strlen(command)-1] = 0;
			// Give the command to SRI and split the resulting string into multiple strings if it is over 1023 characters long
			string results = session->processCommand(command); // Give the command to SRI
			vector<string> out;
			for(unsigned int i = 0; i < results.length(); i += 1023){
				out.push_back(results.substr(i, 1023));
			}
			// Tell the client how many strings are being returned
			string snum = to_string(out.size());
			while(snum.length() < 1023) snum += " "; // Add white space to the end of the string (otherwise valgrind will complain about an illegal read)
			const char * num;
			num = snum.c_str();
			tcpSocket->writeToSocket(num,1023);
			// Write the results to the socket, one string at a time
			for(unsigned int i = 0; i < out.size(); i++){
				string sresult = out[i];
				while(sresult.length() < 1023) sresult += " ";
				const char * result = sresult.c_str();
				tcpSocket->writeToSocket(result,1023);
			}
		}
		else break; // Error reading from the socket or connection closed
	}
	delete(session); // Delete the session
	tcpSocket->shutDown(); // Shutdown the TCP Socket
	cout << "Closed connection " << tcpSocket->getRemoteAddress() << endl;
	return NULL;
}