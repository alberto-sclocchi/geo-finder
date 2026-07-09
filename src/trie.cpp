#include "trie.h"
#include <cctype>

Trie::~Trie()
{
    for (int i = 0; i < 26; i++) {
        if(root->children[i] != nullptr) {
            delete root->children[i];
            root->children[i] = nullptr;
        }
    }

    delete root;
    root = nullptr;

    delete this;
}

void Trie::insert(const string& word)
{
    TrieNode* current = root;

    for (char c : word) {
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
}

bool Trie::search(const string& word)
{
    TrieNode* current = root;

    for (char c : word) {
        int index = static_cast<char>(tolower(c)) - 'a'; //find the index of the child node
        if(index < 0 || index >= 26) continue; //skip invalid characters

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return false;

        //if not null, move current ptr to the child node
        current = current->children[index];
    }

    //return the value of isWord for the current node
    return current->isWord;
}

vector<string> Trie::autocomplete(const string& prefix) //return a list of strings that start with the given prefix
{
    vector<string> results;
    if(prefix.empty() || root == nullptr) return results;

    //find the node with the prefix
    TrieNode* current = root;
    for (char c : prefix) {
        int index = static_cast<char>(tolower(c)) - 'a';
        if (index < 0 || index >= 26) continue;

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return results;

        //move the current pointer to the child node
        current = current->children[index];
    }

    //call the helper function to find all words with the given prefix
    autocompleteHelper(prefix, current, results);
    return results;
}

void Trie::autocompleteHelper(const string& prefix, TrieNode* node, vector<string>& results)
{
    if(node == nullptr) return;

    // check if the current node is a complete word and add it to the result vector
    if (node->isWord) {
        results.push_back(prefix);
    }

    //recursively explore all child nodes for each letter in the alphabet using DFS
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            char c = 'a' + i;
            autocompleteHelper(prefix + c, node->children[i], results);
        }
    }
}
