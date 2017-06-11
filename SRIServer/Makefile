
#------------------------------------------------------------------------------
# SRI Makefile
#
#       make                  makes SRI
#		make clean			  deletes SRI executable
#       make check            runs valgrind on SRI
#------------------------------------------------------------------------------

OBJ = main.o common.o Session.o Statement.o Fact.o Rule.o ANDRule.o ORRule.o Name.o Group.o TCPSocket.o TCPServerSocket.o Connection.o GarbageCollector.o Thread.o ThreadManager.o ORThread.o ANDThread.o SessionThread.o
HEADER = include/includes.h include/common.h include/Session.h include/Statement.h include/Fact.h include/Rule.h include/ANDRule.h include/ORRule.h include/Name.h include/Group.h include/TCPSocket.h include/TCPServerSocket.h include/Connection.h include/GarbageCollector.h include/Thread.h include/ThreadManager.h include/ORThread.h include/ANDThread.h include/SessionThread.h

SRI: $(OBJ)
	g++ -std=c++11 $(OBJ) -o $@ -lpthread
	
main.o: main.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
common.o: src/common.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Session.o: src/Session.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Statement.o: src/Statement.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Fact.o: src/Fact.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Rule.o: src/Rule.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
ANDRule.o: src/ANDRule.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
ORRule.o: src/ORRule.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Name.o: src/Name.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Group.o: src/Group.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
TCPSocket.o: src/TCPSocket.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
TCPServerSocket.o: src/TCPServerSocket.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Connection.o: src/Connection.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
GarbageCollector.o: src/GarbageCollector.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
Thread.o: src/Thread.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
ThreadManager.o: src/ThreadManager.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
ORThread.o: src/ORThread.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
ANDThread.o: src/ANDThread.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
SessionThread.o: src/SessionThread.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
clean:
	rm -f *o SRI
	
check:
	valgrind --leak-check=full SRI