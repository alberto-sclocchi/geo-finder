#pragma once
#include <iostream>
#include <vector>
#include <string>

enum Color { RED, BLACK };

struct Node {
    std::string key;
    Color color;
    Node* left;
    Node* right;
    Node* parent;
    Node(std::string k);
};

class RedBlackTree {
private:
    Node* root;
    void rotateLeft(Node* x);
    void rotateRight(Node* x);  
public:
    RedBlackTree();
    ~RedBlackTree();
    bool isRed(Node* node) {};
    void insertFixup(Node* z);
    Node* searchNode(std::string key);
    void insert(std::string key);
    vector<Node*> prefixSearch(std::string prefix);
};
