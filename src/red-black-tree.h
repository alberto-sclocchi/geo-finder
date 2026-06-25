#pragma once
#include <iostream>

enum Color { RED, BLACK };

struct RBNode {
    int key;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode(int k);
};

class RedBlackTree {
private:
    RBNode* root;  
    void rotateLeft(RBNode* x);
    void rotateRight(RBNode* x);  
public:
    RedBlackTree();
    ~RedBlackTree();
    RBNode* searchNode(int key);
    void insert(int key);   
    void inorder();  
};
