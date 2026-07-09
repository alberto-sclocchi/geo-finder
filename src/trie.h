#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "place.h"

using namespace std; 

struct TrieNode {
    array<TrieNode*, 26> children;
    vector<Place> places;
    bool isWord;

    TrieNode() : isWord(false) {
        children.fill(nullptr);
    }
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() : root(new TrieNode()) {};
    ~Trie();
    
    void insert(const string& word);
    bool search(const string& word);

    vector<string> autocomplete(const string& prefix, vector<string>& places);
    void autocompleteHelper(const string& prefix, TrieNode* node, vector<string>& places);
};