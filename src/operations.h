#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "place.h"
#include "red-black-tree.h"
#include "trie.h"


void loadData(const string& filename, vector<Place>& data);

void displayPrefixResults(const string& input, const vector<Place>& results, long long timeNs);

void displaySearchResults(const string& input, const vector<Place>& results, long long timeNs);