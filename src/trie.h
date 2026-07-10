#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include "place.h"

using namespace std;

const string TRIE_ALPHABET = " &'(),-./0123456789?[]`abcdefghijklmnopqrstuvwxyz";
const int TRIE_ALPHABET_SIZE = 49;

struct TrieNode {
    array<TrieNode*, TRIE_ALPHABET_SIZE> children;
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
    Trie() : root(new TrieNode()) {}
    ~Trie();
    void destroy(TrieNode* node);

    void insert(const Place& place);
    vector<Place> search(const string& word);

    vector<Place> autocomplete(const string& prefix);
    void autocompleteHelper(TrieNode* node, vector<Place>& result);
};
