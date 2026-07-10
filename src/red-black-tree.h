#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "place.h"

enum Color { RED, BLACK };

struct Node {
    vector<Place> places;
    string key; 
    Color color;
    Node* left;
    Node* right;
    Node* parent;
    Node(vector<Place> places);
};

class RedBlackTree {
private:
    Node* root;
    void rotateLeft(Node* x);
    void rotateRight(Node* x);  
public:
    RedBlackTree();
    ~RedBlackTree();
    bool isRed(Node* node);
    void insertFixup(Node* z);
    vector<Place> searchNode(string place);
    void insert(Place place);
    vector<Place> prefixSearch(string prefix);
};
