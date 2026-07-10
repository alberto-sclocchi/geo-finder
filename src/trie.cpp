#include "trie.h"
#include <cctype>
#include <algorithm>
#include "operations.h"

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

void Trie::insert(const Place& p)
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

vector<Place> Trie::search(const string& place)
{
    vector<Place> results;
    TrieNode* current = root;

    for (char c : place) {
        int index = static_cast<char>(tolower(c)) - 'a'; //find the index of the child node
        if(index < 0 || index >= 26) continue; //skip invalid characters

        //if the child node at the index in the children array is null
        if (current->children[index] == nullptr) return results;

        //if not null, move current ptr to the child node
        current = current->children[index];
    }

    string key = toLowerName(place);
    for (const Place& p : current->places) {
        if (toLowerName(p.asciiName) == key) {
            results.push_back(p);
        }
    }
    return results;
}

bool byLowerName(const Place& a, const Place& b) {
    return toLowerName(a.asciiName) < toLowerName(b.asciiName);
}

vector<Place> Trie::autocomplete(const string& prefix) //return the places whose name starts with the given prefix
{
    vector<Place> results;
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

    //collect every place stored under this node
    vector<Place> collected;
    autocompleteHelper(current, collected);

    //the walk above skips non-letter characters, so keep only places whose
    //real name starts with the typed prefix (case-insensitive)
    string key = toLowerName(prefix);
    for (const Place& p : collected) {
        if (toLowerName(p.asciiName).compare(0, key.size(), key) == 0) {
            results.push_back(p);
        }
    }

    //sort by name so the output matches the red-black tree's in-order results,
    stable_sort(results.begin(), results.end(), byLowerName);

    //resize the results to the limit if there are more than PREFIX_RESULT_LIMIT results
    if (results.size() > PREFIX_RESULT_LIMIT) {
        results.resize(PREFIX_RESULT_LIMIT);
    }
    return results;
}

void Trie::autocompleteHelper(TrieNode* node, vector<Place>& result)
{
    if(node == nullptr) return;

    //a node marked as a word holds the records of at least one complete name
    if (node->isWord) {
        result.insert(result.end(), node->places.begin(), node->places.end());
    }

    //recursively explore all child nodes for each letter in the alphabet using DFS
    for (int i = 0; i < 26; i++) {
        if (node->children[i] != nullptr) {
            autocompleteHelper(node->children[i], result);
        }
    }
}
