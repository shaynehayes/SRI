SRI is a multithreaded C++ program capable of making logical inferences based on groups of facts and rules. It was written by Shayne Hayes, Son Huynh, Richard Harker, and Nicole Maines for CMPS109 (Advanced Programming). Here is a list of commands and what they do:

FACT Name(name, name, ...) -- Defines a relation between a group of names.
Ex. FACT Father(John, Mary)
RULE Name($param, $param, ...) operator Predicate($param, $param, ...) Predicate($param, $param, ...) -- Defines a logical rule between a group of predicates.
Ex. RULE Parent($X,$Y) OR Father($X,$Y) Mother($X,$Y)
Ex. RULE GreatGrandfather($A,$B) AND Father($A,$C) Parent($C,$D) Parent($D,$B)
INFERENCE Name(name/$param, name/$param, ...) -- Issues a query on a fact or rule.
Ex. INFERENCE GreatGrandfather($D, $E)
Ex. INFERENCE Father($X, Mary)
Ex. INFERENCE Parent($X, $X) SelfParent //Add a name after the inference to make a new fact out of the results
DROP Name(name/$param, name/$param, ...) -- Drops a fact or rule.
Ex. DROP Parent
Ex. DROP Trio(Albert, Geoffrey, $B)
LOAD File -- Runs an sri script.
Ex. LOAD yournamehere.sri
DUMP File -- Saves the facts and rules of the current session to an sri file.
Ex. DUMP yournamehere.sri

Included with the client and server source code are a makefile, build instructions for Unix, UML diagrams, and a design report. Thanks to Karim Sobh for the Thread, TCPSocket, TCPServerSocket, Connection, and GarbageCollector utility classes.
