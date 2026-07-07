#pragma once
#include <iostream>

enum Color { RED, BLACK };

struct Node {
    int key;
    Color color;
    Node* left;
    Node* right;
    Node* parent;
    Node(int k);
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
    Node* searchNode(int key);
    void insert(int key);   
};
