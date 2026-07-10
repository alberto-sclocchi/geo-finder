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

void Trie::insert(Place p)
{
    TrieNode* current = root;

    for (char c : p.asciiName) {
        int index = static_cast<char>(tolower(c)) - 'a'; //find the index of the child node
        if(index < 0 || index >= 26) continue; //skip invalid characters

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

Place Trie::search(const string& word)
{
    TrieNode* current = root;

    for (char c : word) {
        int index = static_cast<char>(tolower(c)) - 'a'; //find the index of the child node
        if(index < 0 || index >= 26) continue; //skip invalid characters

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return Place();

        //if not null, move current ptr to the child node
        current = current->children[index];
    }

    //return the value of isWord for the current node
    return current->isWord ? current->places[0] : Place();
}

vector<string> Trie::autocomplete(const string& prefix) //return a list of strings that start with the given prefix
{
    vector<string> places;
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
    autocompleteHelper(prefix, current, places);
    return places;
}

void Trie::autocompleteHelper(const string& prefix, TrieNode* node, vector<string>& result)
{
    if(node == nullptr) return;

    // check if the current node is a complete word and add it to the result vector
    if (node->isWord) {
        result.push_back(prefix);
    }

    //recursively explore all child nodes for each letter in the alphabet using DFS
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            char c = 'a' + i;
            autocompleteHelper(prefix + c, node->children[i], result);
        }
    }
}
