#include "red-black-tree.h"
#include <string>
#include <vector>

using namespace std;

const int COUNT = 5; // number of nodes to return for prefix search

Node::Node(vector<Place> _places): places(_places), color(RED), left(nullptr), right(nullptr), parent(nullptr){}

RedBlackTree::RedBlackTree(): root(nullptr) {}

RedBlackTree::~RedBlackTree() {
}

bool RedBlackTree::isRed(Node* node) {
    return node != nullptr && node->color == RED;
}

void RedBlackTree::rotateLeft(Node* x) {
    // perform left rotation around node x
    Node* y = x->right;
    x->right = y->left;

    // update parent of y's left child if it exists
    if (y->left != nullptr) {
        y->left->parent = x;
    }

    // update parent of y
    y->parent = x->parent;

    if (x->parent == nullptr) {
        // update the root if x was the root
        root = y;
    } else if (x == x->parent->left) {
        // update the left child of x's parent
        x->parent->left = y;
    } else {
        // update the right child of x's parent
        x->parent->right = y;
    }

    // complete the rotation
    y->left = x;

    // update the parent of x
    x->parent = y;
}

void RedBlackTree::rotateRight(Node* x) {
    // perform right rotation around node x
    Node* y = x->left;
    x->left = y->right;

    // update parent of y's right child if it exists
    if (y->right != nullptr) {
        y->right->parent = x;
    }

    // update parent of y
    y->parent = x->parent;


    if (x->parent == nullptr) {
        // update the root if x was the root
        root = y;
    } else if (x == x->parent->right) {
        // update the right child of x's parent
        x->parent->right = y;
    } else {
        // update the left child of x's parent
        x->parent->left = y;
    }

    // complete the rotation
    y->right = x;

    // update the parent of x
    x->parent = y;
}


void RedBlackTree::insertFixup(Node* z) {

    // fix the red-black tree properties after insertion, only if the parent of z is red
    while(isRed(z->parent)){
        if(z->parent == z->parent->parent->left){ // z's parent is a left child
            Node* y = z->parent->parent->right;   // z's uncle
            if(isRed(y)){ // z's uncle is red
                // color flip
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // z's uncle is black
                if(z == z->parent->right){
                    // left right case only, if not then it is a left left case
                    z = z->parent;
                    rotateLeft(z);
                }

                // color flip and right rotation
                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                // right rotation grandparent
                rotateRight(z->parent->parent);
            }
        } else { // z's parent is a right child
            Node* y = z->parent->parent->left; // z's uncle
            if(isRed(y)){ // z's uncle is red
                // color flip
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // z's uncle is black
                if(z == z->parent->left){
                    // right left case only, if not then it is a right right case
                    z = z->parent;
                    rotateRight(z);
                }

                // color flip and left rotation
                z->parent->color = BLACK;
                z->parent->parent->color = RED;

                // left rotation grandparent
                rotateLeft(z->parent->parent);
            }
        }

        if (z == root) {
            break; // if z is the root, exit the loop
        }
    }
}

void RedBlackTree::insert(Place place) {
    vector<Place> places = {place}; // create a vector with the single place
    Node* z = new Node(places);

    Node* y = nullptr;
    Node* x = root;

    while (x != nullptr){
        y = x;
        if (z->places[0].asciiName < x->places[0].asciiName) {
            x = x->left;
        } else if (z->places[0].asciiName > x->places[0].asciiName) {
            x = x->right;
        } else {
            x->places.push_back(place); 
            return;
        }
    }

    z->parent = y; // set the parent of the new node
    if (y == nullptr) {
        root = z;
    } else if (z->places[0].asciiName < y->places[0].asciiName) {
        y->left = z;
    } else {
        y->right = z;   
    }

    insertFixup(z); // fix the tree to maintain red-black properties

    root->color = BLACK; // ensure the root is always black

}

Node* searchNodeHelper(Node* node, string key) {
    if (node == nullptr) {
        return nullptr;
    } else if (key == node->places[0].asciiName) {
        return node;
    } else if (key < node->places[0].asciiName) {
        return searchNodeHelper(node->left, key);
    } else {
        return searchNodeHelper(node->right, key);
    }
}

Place RedBlackTree::searchNode(string key) {
    Node* result = searchNodeHelper(root, key);
    return result != nullptr ? result->places[0] : Place();
}

void prefixSearchHelper(Node* node, string prefix, vector<Place>& result) {
    if (node == nullptr || result.size() > COUNT) {
        return;
    }

    if (node->places[0].asciiName >= prefix) {
        prefixSearchHelper(node->left, prefix, result);
    }

    if(result.size() > COUNT) {
        return; // stop if we have enough results
    }

    // check if the current node's key starts with the prefix
    bool startsWithPrefix = node->places[0].asciiName.compare(0, prefix.size(), prefix) == 0;
    if(startsWithPrefix) {
        for(const auto& place : node->places) {
            result.push_back(place);
        }
    }

    if (node->places[0].asciiName < prefix || startsWithPrefix) {
        prefixSearchHelper(node->right, prefix, result);
    }
}

vector<Place> RedBlackTree::prefixSearch(string prefix) {
    vector<Place> result;
    prefixSearchHelper(root, prefix, result);
    return result;
}
