#include "trie.h"

void Trie::insert(const string& word)
{
    TrieNode* current = root;

    for (char c : word) {
        int index = c - 'a'; //find the index of the child node

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
        int index = c - 'a'; //find the index of the child node

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) { 
            return false;
        }

        //if not null, move current ptr to the child node
        current = current->children[index];
    }

    //return the value of isWord for the current node
    return current->isWord;
}

void Trie::automplete(const string &prefix, vector<string> &results)
{
    
}
