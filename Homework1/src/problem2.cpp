#include <iostream>
#include <algorithm>
#include <cmath>
#include <ctime>

using namespace std;

template <class K, class E>
struct Node {
    K key;
    E element;
    Node<K, E> *left, *right;
    Node(K k, E e) : key(k), element(e), left(nullptr), right(nullptr) {}
};

template <class K, class E>
class BST {
private:
    Node<K, E>* root;

    int getHeight(Node<K, E>* node) {
        if (!node) return 0;
        return 1 + max(getHeight(node->left), getHeight(node->right));
    }

public:
    BST() : root(nullptr) {}

    void Insert(const K& k, const E& e) {
        Node<K, E>** curr = &root;
        while (*curr) {
            if (k < (*curr)->key) curr = &((*curr)->left);
            else if (k > (*curr)->key) curr = &((*curr)->right);
            else return;
        }
        *curr = new Node<K, E>(k, e);
    }

    void Delete(const K& k) {
        Node<K, E> **curr = &root;
        while (*curr && (*curr)->key != k) {
            if (k < (*curr)->key) curr = &((*curr)->left);
            else curr = &((*curr)->right);
        }
        if (!*curr) return;

        Node<K, E>* old = *curr;
        if (!(*curr)->left) *curr = (*curr)->right;
        else if (!(*curr)->right) *curr = (*curr)->left;
        else {
            Node<K, E>** succ = &((*curr)->right);
            while ((*succ)->left) succ = &((*succ)->left);
            old->key = (*succ)->key;
            old->element = (*succ)->element;
            old = *succ;
            *succ = (*succ)->right;
        }
        delete old;
    }

    int Height() { return getHeight(root); }
};
