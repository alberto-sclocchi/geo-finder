#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "place.h"
#include "red-black-tree.h"
#include "trie.h"


void loadData(const string& filename, vector<Place>& data);

string toLowerName(const string& s);