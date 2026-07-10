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

long long benchMarkBuildTrie(const vector<Place>& data, Trie& trie);

long long benchMarkBuildRBTree(const vector<Place>& data, RedBlackTree& tree);

long long benchMarkSearchTrie(Trie& trie, vector<string>& inputs);

long long benchMarkSearchRBTree(RedBlackTree& tree, vector<string>& inputs);

long long benchMarkPrefixSearchTrie(Trie& trie, vector<string>& inputs);

long long benchMarkPrefixSearchRBTree(RedBlackTree& tree, vector<string>& inputs);