#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <typeinfo>
#include <cstdint>
#include <array>
#include <iterator>
#include <map>
#include <sstream>
#include <cstdlib>
#include <sstream>
#include <strings.h>
#include "Group.h"

using namespace std;

struct ignorecase { 
    bool operator() (const std::string& lhs, const std::string& rhs) const {
        return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
    }
};

// compressVector()
// Helper function that compresses a 2D Group vector into a 1D vector, deleting duplicate entries
vector<Group*> compressVector(vector<vector<Group*> > groups);

// isDuplicate()
// Helper function that determines whether or not a group already exists in a vector
bool isDuplicate(Group * g, vector<Group*> groups);

#endif // COMMON_H_
