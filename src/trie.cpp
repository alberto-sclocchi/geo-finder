#include "trie.h"
#include <cctype>

Trie::~Trie()
{
    destroy(root);
}

void Trie::destroy(TrieNode *node)
{
    if(node == nullptr) return;

    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            destroy(node->children[i]);
        }
    }

    delete node;
}

void Trie::insert(const Place& place)
{
    TrieNode* current = root;

    for (char c : place.asciiName) {
        int index = static_cast<char>(tolower(c)) - 'a'; //find the index of the child node
        if(index < 0 || index >= 26) continue; //skip invalid characters

        //if the child node at the index in the children array is null, create a new node at that index
        if (current->children[index] == nullptr) {
            current->children[index] = new TrieNode();
        }

        //move current ptr to the child node
        current = current->children[index];
    }

    //make the last node a word (true)
    current->isWord = true;
    current->places.push_back(place);
}

vector<Place> Trie::search(const string& word)
{
    TrieNode* current = root;

    for (char c : word) {
        int index = static_cast<char>(tolower(c)) - 'a'; //find the index of the child node
        if(index < 0 || index >= 26) continue; //skip invalid characters

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return vector<Place>();

        //if not null, move current ptr to the child node
        current = current->children[index];
    }

    //return the places stored at the current node
    return current->places;
}

vector<Place> Trie::autocomplete(const string& prefix, vector<Place>& places) //return a list of strings that start with the given prefix
{
    if(prefix.empty() || root == nullptr) return places;

    //find the node with the prefix
    TrieNode* current = root;
    for (char c : prefix) {
        int index = static_cast<char>(tolower(c)) - 'a';
        if (index < 0 || index >= 26) continue;

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return places;

        //move the current pointer to the child node
        current = current->children[index];
    }

    //call the helper function to find all words with the given prefix
    autocompleteHelper(current, places);
    return places;
}

void Trie::autocompleteHelper(TrieNode* node, vector<Place>& places)
{
    if(node == nullptr) return;

    // check if the current node is a complete word and add it to the result vector
    if (node->isWord) {
        for (const Place& place : node->places) {
            places.push_back(place);
        }
    }

    //recursively explore all child nodes for each letter in the alphabet using DFS
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            autocompleteHelper(node->children[i], places);
        }
    }
}
