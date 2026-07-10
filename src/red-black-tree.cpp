#include "red-black-tree.h"
#include <string>
#include <vector>
#include "operations.h"

using namespace std;

Node::Node(vector<Place> _places): places(_places), key(toLowerName(_places[0].asciiName)), color(RED), left(nullptr), right(nullptr), parent(nullptr){}

RedBlackTree::RedBlackTree(): root(nullptr) {}

void destroy (Node* node) {
    if (node == nullptr) {
        return;
    }
    destroy(node->left);
    destroy(node->right);
    delete node;
}

RedBlackTree::~RedBlackTree() {
    destroy(root);
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

    // compare lowercase keys so lookups are case-insensitive
    while (x != nullptr){
        y = x;
        if (z->key < x->key) {
            x = x->left;
        } else if (z->key > x->key) {
            x = x->right;
        } else {
            x->places.push_back(place);
            delete z;
            return;
        }
    }

    z->parent = y; // set the parent of the new node
    if (y == nullptr) {
        root = z;
    } else if (z->key < y->key) {
        y->left = z;
    } else {
        y->right = z;
    }

    insertFixup(z); // fix the tree to maintain red-black properties

    root->color = BLACK; // ensure the root is always black

}

Node* searchNodeHelper(Node* node, const string& key) {
    if (node == nullptr) {
        return nullptr;
    } else if (key == node->key) {
        return node;
    } else if (key < node->key) {
        return searchNodeHelper(node->left, key);
    } else {
        return searchNodeHelper(node->right, key);
    }
}

vector<Place> RedBlackTree::searchNode(string key) {
    Node* result = searchNodeHelper(root, toLowerName(key));
    return result != nullptr ? result->places : vector<Place>();
}

void prefixSearchHelper(Node* node, const string& prefix, vector<Place>& result) {
    if (node == nullptr || result.size() >= PREFIX_RESULT_LIMIT) {
        return;
    }

    if (node->key >= prefix) {
        prefixSearchHelper(node->left, prefix, result);
    }

    if(result.size() >= PREFIX_RESULT_LIMIT) {
        return; // stop if we have enough results
    }

    // check if the current node's key starts with the prefix
    bool startsWithPrefix = node->key.compare(0, prefix.size(), prefix) == 0;
    if(startsWithPrefix) {
        for(const auto& place : node->places) {
            if(result.size() >= PREFIX_RESULT_LIMIT) return;
            result.push_back(place);
        }
    }

    if (node->key < prefix || startsWithPrefix) {
        prefixSearchHelper(node->right, prefix, result);
    }
}

vector<Place> RedBlackTree::prefixSearch(string prefix) {
    vector<Place> result;
    if (prefix.empty()) return result; 
    prefixSearchHelper(root, toLowerName(prefix), result);
    return result;
}
