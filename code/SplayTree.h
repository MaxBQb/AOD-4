template<class T>
class SplayTree {
public:
    struct Node {
        Node *left, *right;
        Node *parent;
        T data;
        Node(const T& init = T()):
                left(nullptr),
                right(nullptr),
                parent(nullptr),
                data(init) {}
    };

    SplayTree(): root(nullptr), rotations_count(0) {}

    unsigned int getRotationsCount() { return rotations_count; }
    void insert(const T &key);
    Node* find(const T &key);
    bool remove(const T &key);
    void print(ostream &out, Node *start_node, int level = 0);
    void print(ostream &out) { print(out, root); }
    T findMin() { return findMin(root)->data; }
    T findMax() { return findMax(root)->data; }

private:
    Node* root;
    unsigned rotations_count;

    void rotateLeft(Node *x);
    void rotateRight(Node *x);
    void splay(Node *x);
    void replace(Node *old_node, Node *new_node);
    Node* findMin(Node *start_node);
    Node* findMax(Node *start_node);
};

template<class T>
void SplayTree<T>::rotateLeft(Node *x) {
    rotations_count++;
    Node *r_node = x->right;
    if (r_node) {
        x->right = r_node->left;
        if (r_node->left)
            r_node->left->parent = x;
        r_node->parent = x->parent;
    }

    if (!x->parent)
        root = r_node;
    else if (x == x->parent->left)
        x->parent->left = r_node;
    else
        x->parent->right = r_node;

    if (r_node)
        r_node->left = x;
    x->parent = r_node;
}

template<class T>
void SplayTree<T>::rotateRight(Node *x) {
    rotations_count++;
    Node *l_node = x->left;
    if (l_node) {
        x->left = l_node->right;
        if (l_node->right)
            l_node->right->parent = x;
        l_node->parent = x->parent;
    }
    if (!x->parent)
        root = l_node;
    else if (x == x->parent->left)
        x->parent->left = l_node;
    else
        x->parent->right = l_node;

    if (l_node)
        l_node->right = x;
    x->parent = l_node;
}

template<class T>
void SplayTree<T>::splay(Node *x) {
    while (x->parent) {
        if (!x->parent->parent) {
            if (x->parent->left == x)
                rotateRight(x->parent);
            else
                rotateLeft(x->parent);
        } else if (x->parent->left == x &&
                   x->parent->parent->left == x->parent) {
            rotateRight(x->parent->parent);
            rotateRight(x->parent);
        } else if (x->parent->right == x &&
                   x->parent->parent->right == x->parent) {
            rotateLeft(x->parent->parent);
            rotateLeft(x->parent);
        } else if (x->parent->left == x &&
                   x->parent->parent->right == x->parent) {
            rotateRight(x->parent);
            rotateLeft(x->parent);
        } else {
            rotateLeft(x->parent);
            rotateRight(x->parent);
        }
    }
}

template<class T>
void SplayTree<T>::replace(Node *old_node, Node *new_node) {
    if (!old_node->parent)
        root = new_node;
    else if (old_node == old_node->parent->left)
        old_node->parent->left = new_node;
    else
        old_node->parent->right = new_node;

    if (new_node)
        new_node->parent = old_node->parent;
}

template<class T>
typename SplayTree<T>::Node* SplayTree<T>::findMin(Node *start_node) {
    while (start_node->left)
        start_node = start_node->left;
    return start_node;
}

template<class T>
typename SplayTree<T>::Node* SplayTree<T>::findMax(Node *start_node) {
    while (start_node->right)
        start_node = start_node->right;
    return start_node;
}

template<class T>
void SplayTree<T>::insert(const T &key) {
    Node *current_node = root;
    Node *previous_node = nullptr;

    while (current_node) {
        previous_node = current_node;
        current_node = current_node->data < key ?
                current_node->right :
                current_node->left;
    }

    current_node = new Node(key);
    current_node->parent = previous_node;

    if (!previous_node)
        root = current_node;
    else if (previous_node->data < current_node->data)
        previous_node->right = current_node;
    else
        previous_node->left = current_node;

    splay(current_node);
}

template<class T>
typename SplayTree<T>::Node *SplayTree<T>::find(const T &key) {
    Node *current_node = root;
    while (current_node) {
        if (current_node->data < key)
            current_node = current_node->right;
        else if (key < current_node->data)
            current_node = current_node->left;
        else
            return current_node;
    }
    return nullptr;
}

template<class T>
bool SplayTree<T>::remove(const T &key) {
    Node *selected_node = find(key);
    if (!selected_node)
        return false;

    splay(selected_node);

    if (!selected_node->left)
        replace(selected_node, selected_node->right);
    else if (!selected_node->right)
        replace(selected_node, selected_node->left);
    else {
        Node *y = findMin(selected_node->right);
        if (y->parent != selected_node) {
            replace(y, y->right);
            y->right = selected_node->right;
            y->right->parent = y;
        }
        replace(selected_node, y);
        y->left = selected_node->left;
        y->left->parent = y;
    }

    delete selected_node;
    return true;
}

template<class T>
void SplayTree<T>::print(ostream &out, Node *start_node, int level) {
    if (start_node == nullptr)
        return;
    print(out, start_node->right, level + 1);
    for (int i = 0; i < (level*2); i++)
        out << '\t';
    out << start_node->data << endl;
    print(out, start_node->left, level + 1);
}