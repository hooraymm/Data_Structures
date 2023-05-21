#include <iostream>
#include <algorithm>
#include <optional>
#include <unistd.h>

int Max(int n0, int n1)
{
    if (n0 < n1) return n1;
    else         return n0;
}

template <typename T, typename U>
class AVLNode {

public:
    T key;
    U value;
    AVLNode<T, U>* left;
    AVLNode<T, U>* right;
    int height;

    AVLNode<T, U>(const T& k, const U& v)
    {
        key = k;
        value = v;
        left = nullptr;
        right = nullptr;
        height = 1;
    }

};

template <typename T, typename U>
class AVLTree {
public:
    AVLNode<T, U>* root = nullptr;
    ~AVLTree() {
        removeall(root);
    }

    void insert(const T& key, const U& value);
    U search(const T& key);
    bool remove(const T& key);

    //for checking
    void preorder(AVLNode<T, U>*& node) {
        if (!node) return;

        std::cout << node->key << ": " << node->value << std::endl;
        preorder(node->left);
        preorder(node->right);
    }
    void inorder(AVLNode<T, U>*& node) {
        if (!node) return;

        inorder(node->left);
        std::cout << node->key << ": " << node->value << std::endl;
        inorder(node->right);
    }

private:
    int getHeight(AVLNode<T, U>*& node);
    int getBalance(AVLNode<T, U>*& node);
    AVLNode<T, U>* rotate_left(AVLNode<T, U>*& node);
    AVLNode<T, U>* rotate_right(AVLNode<T, U>*& node);

    AVLNode<T, U>* insert(AVLNode<T, U>*& node, const T& key, const U& value);
    U search(AVLNode<T, U>*& node, const T& key);
    AVLNode<T, U>* remove(AVLNode<T, U>*& node, const T& key);
    void removeall(AVLNode<T, U>*& node);

};

template <typename T, typename U>
int AVLTree<T, U>::getHeight(AVLNode<T, U>*& node) {
    if (!node) return 0;
    return node->height;
}

template <typename T, typename U>
int AVLTree<T, U>::getBalance(AVLNode<T, U>*& node) {
    if (!node) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template<typename T, typename U>
void AVLTree<T, U>::insert(const T& key, const U& value) {
    root = insert(root, key, value);
}

template<typename T, typename U>
U AVLTree<T, U>::search(const T& key) {
    return search(root, key);
}

template<typename T, typename U>
bool AVLTree<T, U>::remove(const T& key) {
    if (!search(root, key).length()) return false;
    root = remove(root, key);
    return true;
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::rotate_left(AVLNode<T, U>*& node) {
    if (node->right == nullptr) {
        return node;
    }

    AVLNode<T, U>* pNode1 = node->right;
    AVLNode<T, U>* pNode2 = node->right->left;
    AVLNode<T, U>* pNode3 = node;

    pNode1->left = pNode3;
    pNode3->right = pNode2;

    pNode3->height = 1 + Max(getHeight(pNode3->left), getHeight(pNode3->right));
    pNode1->height = 1 + Max(getHeight(pNode1->left), getHeight(pNode1->right));

    return pNode1;
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::rotate_right(AVLNode<T, U>*& node) {
    if (node->left == nullptr) {
        return node;
    }

    AVLNode<T, U>* pNode1 = node->left;
    AVLNode<T, U>* pNode2 = node->left->right;
    AVLNode<T, U>* pNode3 = node;

    pNode1->right = pNode3;
    pNode3->left = pNode2;

    pNode3->height = 1 + Max(getHeight(pNode3->left), getHeight(pNode3->right));
    pNode1->height = 1 + Max(getHeight(pNode1->left), getHeight(pNode1->right));

    return pNode1;
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::insert(AVLNode<T, U>*& node, const T& key, const U& value) {

    if (node == nullptr) {
        AVLNode<T, U>* newNode = new AVLNode<T, U>(key, value);
        return newNode;
    }

    if (node->key > key) {
        node->left = insert(node->left, key, value);
    }
    else if (node->key < key) {
        node->right = insert(node->right, key, value);
    }
    else {
        return node;
    }

    node->height = 1 + Max(getHeight(node->left), getHeight(node->right));

    //--------------------
    int balanceFactor = getBalance(node);

    // LL case
    if (balanceFactor > 1 && key < node->left->key) {
        return rotate_right(node);
    }

    // RR case
    if (balanceFactor < -1 && key > node->right->key) {
        return rotate_left(node);
    }

    // LR case
    if (balanceFactor > 1 && key > node->left->key) {

        node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    // RL case
    if (balanceFactor < -1 && key < node->right->key) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

template<typename T, typename U>
U AVLTree<T, U>::search(AVLNode<T, U>*& node, const T& key) {
    //TODO
    //return NULL if there are no such key, return value if there is
    if (node->key > key)
    {
        if (node->left) {
            return search(node->left, key);
        }
    }
    else if (node->key < key)
    {
        if (node->right) {
            return search(node->right, key);
        }
    }
    else {
        return node->value;
    }

    return U();
}

template<typename T, typename U>
AVLNode<T, U>* AVLTree<T, U>::remove(AVLNode<T, U>*& node, const T& key) {
    if (node == nullptr)
        return node;

    if (node->key > key)
    {
        if (node->left)
            node->left = remove(node->left, key);
    }
    else if (node->key < key)
    {
        if (node->right)
            node->right = remove(node->right, key);
    }
    else {
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode<T, U>* pNode = (node->left) ? node->left : node->right;

            if (pNode == nullptr) {
                pNode = node;
                node = nullptr;
            }
            else
                *node = *pNode;

            delete pNode;
        }
        else {

            AVLNode<T, U>* pMinNode = node->right;

            while (pMinNode->left) {
                pMinNode = pMinNode->left;
            }

            node->key = pMinNode->key;
            node->right = remove(node->right, pMinNode->key);
        }
    }

    if (node == nullptr) {
        return node;
    }

    node->height = 1 + Max(getHeight(node->left), getHeight(node->right));
    int balanceFactor = getBalance(node);

    // LL case
    if (balanceFactor > 1 && getBalance(node->left) >= 0) {
        return rotate_right(node);
    }

    // LR case
    if (balanceFactor > 1 && getBalance(node->left) < 0) {

        node->left = rotate_left(node->left);
        return rotate_right(node);
    }


    // RR case
    if (balanceFactor < -1 && getBalance(node->right) <= 0) {
        return rotate_left(node);
    }

    // RL case
    if (balanceFactor < -1 && getBalance(node->right) > 0) {
        node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    return node;
}

template<typename T, typename U>
void AVLTree<T, U>::removeall(AVLNode<T, U>*& node) {
    if (node->left)
        removeall(node->left);

    if (node->right)
        removeall(node->right);

    delete node;
}
