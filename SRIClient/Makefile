
#------------------------------------------------------------------------------
# SRI Makefile
#
#       make                  makes SRI
#		make clean			  deletes SRI executable
#       make check            runs valgrind on SRI
#------------------------------------------------------------------------------

OBJ = main.o TCPSocket.o
HEADER = include/includes.h include/common.h include/TCPSocket.h

SRI: $(OBJ)
	g++ -std=c++11 $(OBJ) -o $@
	
main.o: main.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
TCPSocket.o: src/TCPSocket.cpp $(HEADER)
	g++ -c -Wall -Iinclude -std=c++11 $< -o $@
	
clean:
	rm -f *o SRI
	
check:
	valgrind --leak-check=full SRI