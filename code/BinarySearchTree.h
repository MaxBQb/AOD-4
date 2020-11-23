#pragma once
#include <iostream>
using namespace std;

template<class T>
class BinarySearchTree {
public:
    struct Node {
        Node *left,
             *right;
        T data;
        Node(const T& data) :
                left(nullptr),
                right(nullptr),
                data(data) {}
    };

    BinarySearchTree(): root(nullptr) {}

    void insert(const T& value);
    bool remove(const T& value);
    Node* eject(Node* start_node, T value);
    Node* find(const T &value, Node *start_node);
    Node* findMin(Node* start_node);
    Node* findMax(Node* start_node);
    void print(ostream &out, Node *start_node, int level = 0);
    Node* find(const T &value) { return find(value, root); }
    Node* eject(T value) { return eject(root, value); }
    Node* findMin() { return findMin(root); }
    Node* findMax() { return findMax(root); }
    void print(ostream &out) { print(out, root); }
    Node* getRoot() { return root; }
    ~BinarySearchTree() { removeNode(root); }

private:
    Node* root;
    void removeNode(Node* start_node);
};

template<class T>
void BinarySearchTree<T>::insert(const T &value) {
    auto *new_node = new Node(value);
    Node* current_node;
    Node* previous_node = nullptr;

    current_node = root;
    while (current_node != nullptr) {
        previous_node = current_node;
        if (value < current_node->data)
            current_node = current_node->left;
        else
            current_node = current_node->right;
    }
    if (previous_node == nullptr)
        root = new_node;
    else {
        if (value < previous_node->data)
            previous_node->left = new_node;
        else
            previous_node->right = new_node;
    }
}

template<class T>
bool BinarySearchTree<T>::remove(const T &value) {
    Node* tmp = eject(value);
    return tmp != nullptr;
}

template<class T>
typename BinarySearchTree<T>::Node* BinarySearchTree<T>::eject(BinarySearchTree::Node *start_node, T value) {
    if (start_node == nullptr)
        return start_node;

    if (value < start_node->data)
        start_node->left = eject(start_node->left, value);
    else if (value > start_node->data)
        start_node->right = eject(start_node->right, value);
    else {
        Node* tmp;
        if (start_node->left == nullptr) {
            tmp = start_node->right;
            delete start_node;
            return tmp;
        }
        if (start_node->right == nullptr) {
            tmp = start_node->left;
            delete start_node;
            return tmp;
        }
        tmp = findMin(start_node->right);
        start_node->data = tmp->data;
        start_node->right = eject(start_node->right, tmp->data);
    }
    return start_node;
}

template<class T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::find(const T &value, BinarySearchTree::Node *start_node) {
    if (start_node == nullptr || value == start_node->data)
        return start_node;
    if (value > start_node->data)
        return find(value, start_node->right);
    return find(value, start_node->left);
}

template<class T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::findMin(BinarySearchTree::Node *start_node) {
    if (start_node == nullptr)
        return nullptr;
    Node* current_node = start_node;
    while (current_node->left != nullptr)
        current_node = current_node->left;
    return current_node;
}

template<class T>
typename BinarySearchTree<T>::Node *BinarySearchTree<T>::findMax(BinarySearchTree::Node *start_node) {
    if (start_node == nullptr)
        return nullptr;
    Node* current_node = start_node;
    while (current_node->right != nullptr)
        current_node = current_node->right;
    return current_node;
}

template<class T>
void BinarySearchTree<T>::removeNode(BinarySearchTree::Node *start_node) {
    if (start_node == nullptr)
        return;
    removeNode(start_node->left);
    removeNode(start_node->right);
    delete start_node;
}

template<class T>
void BinarySearchTree<T>::print(ostream &out, Node *start_node, int level) {
    if (start_node == nullptr)
        return;
    print(out, start_node->right, level + 1);
    for (int i = 0; i < (level*2); i++)
        out << '\t';
    out << start_node->data << endl;
    print(out, start_node->left, level + 1);
}
