#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "Place.h"

using namespace std; 

struct TrieNode {
    array<TrieNode*, 26> children;
    vector<Place> children;
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

    void insert(const string& word);
    bool search(const string& word);
    void automplete(const string& prefix, vector<string>& results);
};