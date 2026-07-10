#include "trie.h"

int charToIndex(char c) {
    return TRIE_ALPHABET.find(c);
}

Trie::~Trie()
{
    destroy(root);
}

void Trie::destroy(TrieNode *node)
{
    if(node == nullptr) return;

    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        if (node->children[i] != nullptr) {
            destroy(node->children[i]);
        }
    }

    delete node;
}

void Trie::insert(const Place& p)
{
    TrieNode* current = root;

    for (char c : p.lowerName) {
        int index = charToIndex(c);
        if(index < 0) continue; //skip characters outside the alphabet

        //if the child node at the index in the children array is null, create a new node at that index
        if (current->children[index] == nullptr) {
            current->children[index] = new TrieNode();
        }

        //move current ptr to the child node
        current = current->children[index];
    }

    current->places.push_back(p);
    //make the last node a word (true)
    current->isWord = true;
}

vector<Place> Trie::search(const string& place)
{
    vector<Place> results;
    string key = toLowerName(place); //lowercase the query once, then walk it
    TrieNode* current = root;

    for (char c : key) {
        int index = charToIndex(c); //find the index of the child node
        if(index < 0) return results; //no stored name contains this character

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return results;

        //if not null, move current ptr to the child node
        current = current->children[index];
    }

    if (current->isWord) {
        results = current->places;
    }
    return results;
}

vector<Place> Trie::autocomplete(const string& prefix) //return the places whose name starts with the given prefix
{
    vector<Place> results;
    if(prefix.empty() || root == nullptr) return results;

    string key = toLowerName(prefix); //lowercase the query once, then walk it

    //find the node with the prefix
    TrieNode* current = root;
    for (char c : key) {
        int index = charToIndex(c);
        if (index < 0) return results; //no stored name contains this character

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return results;

        //move the current pointer to the child node
        current = current->children[index];
    }

    autocompleteHelper(current, results);
    return results;
}

void Trie::autocompleteHelper(TrieNode* node, vector<Place>& result)
{
    if(node == nullptr || result.size() >= PREFIX_RESULT_LIMIT) return;

    //a node marked as a word holds the records of at least one complete name
    if (node->isWord) {
        for (const Place& p : node->places) {
            if (result.size() >= PREFIX_RESULT_LIMIT) return;
            result.push_back(p);
        }
    }

    //recursively explore all child nodes in alphabet order using DFS
    for (int i = 0; i < TRIE_ALPHABET_SIZE; i++) {
        
        if (result.size() >= PREFIX_RESULT_LIMIT) return;

        if (node->children[i] != nullptr) {
            autocompleteHelper(node->children[i], result);
        }
    }
}
