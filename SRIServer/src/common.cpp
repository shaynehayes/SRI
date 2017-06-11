
/** 
	common.cpp
	A collection of helper functions/odds and ends
*/

#include "common.h"

using namespace std;

// compressVector()
// Helper function that compresses a 2D Group vector into a 1D vector, deleting duplicate entries
vector<Group*> compressVector(vector<vector<Group*> > groups){
	vector<Group*> out;
	for(unsigned int i = 0; i < groups.size(); i++){
		for(unsigned int j = 0; j < groups[i].size(); j++){
			if(!isDuplicate(groups[i][j], out)) out.push_back(groups[i][j]); // If it is not a duplicate, add it to out
			else delete(groups[i][j]); // If it is a duplicate, delete it and forget about it
		}
	}
	return out;
}

// isDuplicate()
// Helper function that determines whether or not a group already exists in a vector
bool isDuplicate(Group * g, vector<Group*> groups){
	// Determine whether or not the entry is a duplicate
	bool duplicate = false;
	for(unsigned int d = 0; d < groups.size(); d++){
		if(g->equals(groups[d])){duplicate = true; break;}
	}
	return duplicate;
}