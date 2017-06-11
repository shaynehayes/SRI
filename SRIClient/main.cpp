
/** 
	main.cpp
*/

#include <string>
#include "stdlib.h"
#include "iostream"
#include <fstream>

#include "TCPSocket.h"

using namespace std;

void processCommand(TCPSocket * tcpSocket, string in); // Function prototype

string getResults(TCPSocket * tcpSocket, string command){
	tcpSocket->writeToSocket(command.c_str(), command.length()); // Send input to the server
	string out = ""; // Create output string
	char temp[1024]; // Create temporary output buffer
	memset(temp,0,1024); // Initialize the buffer
	tcpSocket->readFromSocket(temp, 1023); // Fetch number of strings being returned by server
	int num = atoi(temp);
	// Fetch the strings one at a time and add them to out
	for(int i = 0; i < num; i++){
		tcpSocket->readFromSocket(temp, 1023); // Fetch the next string
		string trim = temp; // Trim any right white spaces
		trim = trim.substr(0, trim.find_last_not_of(" ")+1);
		out += trim; // Concatenate the results
	}
	return out;
}

void processLoad(TCPSocket * tcpSocket, string fileName){
	ifstream file(fileName.c_str()); // Open file and execute each command
	string in = "";
	if(file.is_open()){
		while(getline(file,in)) if(!in.empty()) processCommand(tcpSocket, in);
		file.close();
		file.clear();
	}
	else cout << "[Error] Unable to open file " << fileName << endl;
}

void processDump(TCPSocket * tcpSocket, string fileName){
	ofstream file(fileName.c_str()); // Open file and dump the KB and RB
	if(file.is_open()){
		string out = getResults(tcpSocket,"DUMP");
		file << out;
		file.close();
		file.clear();
	}
	else cout << "[Error] Unable to open file " << fileName << endl;
}

void processCommand(TCPSocket * tcpSocket, string in){
	string command = in.substr(0,4);
	if(!command.compare("LOAD") || !command.compare("Load") || !command.compare("load")){ // LOAD Command
		if(in.length() > 5) processLoad(tcpSocket, in.substr(5, in.length()));
		else cout << "Usage: LOAD File" << endl;
	}
	else if(!command.compare("DUMP") || !command.compare("Dump") || !command.compare("dump")){ // DUMP Command
		if(in.length() > 5) processDump(tcpSocket, in.substr(5, in.length()));
		else cout << "Usage: DUMP File" << endl;
	}
	else{ // FACT, RULE, INFERENCE, DROP Commands
		string out = getResults(tcpSocket, in);
		cout << out;
	}
}

int loop (TCPSocket * tcpSocket){
	cout << "Command: ";
	string in;
	getline(cin,in); // Get user input
	if(tcpSocket->isPeerDisconnected()){
		cout << "[Error] Connection to the server dropped. Program will now terminate." << endl;
		return 0; // Disconnected from server
	}
	if(in.length() > 3){
		string command = in.substr(0,4);
		if(!command.compare("EXIT") || !command.compare("Exit") || !command.compare("exit")) return 0; // EXIT Command
		else if(!command.compare("LIST") || !command.compare("List") || !command.compare("list")){ // LIST Command
			cout << "EXIT" << endl <<
			"LOAD File" << endl <<
			"DUMP File" << endl <<
			"FACT Name(Name1,Name2,...)" << endl <<
			"RULE Name($Param1,$Param2,...):- OR/AND Predicate1($Param1,$Param2,...) ..." << endl <<
			"INFERENCE Name(Name1/$Param1,Name2/$Param2,...) SaveName(optional)" << endl <<
			"DROP Name1 Name2 ..." << endl;
		}
		else processCommand(tcpSocket, in);
	}
	loop(tcpSocket); // Loop
	return 0;
}

int main(int argc, char *argv[]){
	string ip;
	cout << "\n[Welcome to SRI!] Please enter an IP address." << endl;
	getline(cin,ip);
	char * cip = new char[ip.length()+1];
	strcpy(cip,ip.c_str());
	// Create new TCPSocket
	TCPSocket * tcpSocket = new TCPSocket(cip,9999,1024,1024);
	// If connected to server
	if(!tcpSocket->isPeerDisconnected()){
		cout << "Connection successful! Type LIST for a list of commands." << endl;
		loop(tcpSocket);
	}
	else{ // Connection unsuccessful
		cout << "[Error] Could not connect to " << ip << ". Program will now terminate." << endl;
	}
	delete(tcpSocket);
	delete[] cip;
}