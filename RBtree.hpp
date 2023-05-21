#include <iostream>
#include <algorithm>
#include <optional>
#include <unistd.h>
using namespace std;

#define RED 1
#define BLACK 0

template <typename T, typename U>
class RBNode {

public:
    T key;
    U value;
    RBNode<T, U>* parent;
    RBNode<T, U>* left;
    RBNode<T, U>* right;
    int color; // 1 -> red, 0 -> black

    RBNode<T, U>(const T& k, const U& v)
    {
        key = k;
        value = v;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
        color = BLACK;
    }

};

template <typename T, typename U>
RBNode<T, U>* sib;

template <typename T, typename U>
class RBTree {
public:
    RBNode<T, U>* root = nullptr;
    RBTree()
    {
        sib<T, U> = new RBNode<T, U>(T(), U());
        sib<T, U>->color = BLACK;
    }

    ~RBTree() {
        removeall(root);
        delete sib<T, U>;
    }

    void insert(const T& key, const U& value);
    U search(const T& key);
    bool remove(const T& key);

    void preorder(RBNode<T, U>*& node) {
        if (node == sib<T, U>) return;

        std::cout << node->key << ": " << node->value << std::endl;
        preorder(node->left);
        preorder(node->right);
    }
    void inorder(RBNode<T, U>*& node) {
        if (node == sib<T, U>) return;

        inorder(node->left);
        std::cout << node->key << ": " << node->value << std::endl;
        inorder(node->right);
    }

private:
    RBNode<T, U>* rotate_left(RBNode<T, U>*& node);
    RBNode<T, U>* rotate_right(RBNode<T, U>*& node);
    RBNode<T, U>* insert(RBNode<T, U>*& node, const T& key, const U& value);

    U search(RBNode<T, U>*& node, const T& key);
    RBNode<T, U>* remove(RBNode<T, U>*& node, const T& key);
    void removeall(RBNode<T, U>*& node);

    RBNode<T, U>* searchNode(RBNode<T, U>*& node, const T& key);
};

template<typename T, typename U>
void RBTree<T, U>::insert(const T& key, const U& value) {
    if (root == nullptr) {
        root = new RBNode<T, U>(key, value);
        root->left = sib<T, U>;
        root->right = sib<T, U>;
    }
    else {
        insert(root, key, value);
    }
}

template<typename T, typename U>
U RBTree<T, U>::search(const T& key) {
    return search(root, key);
}

template<typename T, typename U>
bool RBTree<T, U>::remove(const T& key) {
    if (!search(root, key).length())
        return false;

    RBNode<T, U>* removedNode = remove(root, key);
    delete removedNode;

    return true;
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::rotate_left(RBNode<T, U>*& node) {
    RBNode<T, U>* parent = node->parent;
    RBNode<T, U>* rchild = parent->right;

    parent->right = rchild->left;

    if (rchild->left != sib<T, U>) {
        rchild->left->parent = parent;
    }

    rchild->parent = parent->parent;

    if (parent->parent == nullptr) {
        root = rchild;
    }
    else
    {
        if (parent == parent->parent->left) {
            parent->parent->left = rchild;
        }
        else {
            parent->parent->right = rchild;
        }
    }

    rchild->left = parent;
    parent->parent = rchild;

    return parent->right;
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::rotate_right(RBNode<T, U>*& node) {
    RBNode<T, U>* parent = node->parent;
    RBNode<T, U>* lchild = parent->left;

    parent->left = lchild->right;

    if (lchild->right != sib<T, U>) {
        lchild->right->parent = parent;
    }

    lchild->parent = parent->parent;

    if (parent->parent == nullptr) {
        root = lchild;
    }
    else {
        if (parent == parent->parent->left) {
            parent->parent->left = lchild;
        }
        else {
            parent->parent->right = lchild;
        }
    }

    lchild->right = parent;
    parent->parent = lchild;

    return parent->left;
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::insert(RBNode<T, U>*& node, const T& key, const U& value)
{
#if 1
    if (node->key < key) {
        if (node->right == sib<T, U>) {
            RBNode<T, U>* newNode = new RBNode<T, U>(key, value);
            node->right = newNode;
            newNode->parent = node;

            newNode->left = sib<T, U>;
            newNode->right = sib<T, U>;
            newNode->color = RED;

            if (root->parent) {
                RBNode<T, U>* uncleNode = nullptr;
                while (newNode != (root) && newNode->parent->color == RED) {
                    if (newNode->parent == newNode->parent->parent->right) {
                        uncleNode = newNode->parent->parent->left;
                        if (uncleNode->color == RED) {
                            newNode->parent->color = BLACK;
                            uncleNode->color = BLACK;
                            newNode->parent->parent->color = RED;
                            newNode = newNode->parent->parent;
                        }
                        else {
                            if (newNode == newNode->parent->left) {
                                newNode = newNode->parent;
                                rotate_right(newNode);
                            }

                            newNode->parent->color = BLACK;
                            newNode->parent->parent->color = RED;
                            rotate_left(newNode->parent->parent);
                        }
                    }
                    else {
                        uncleNode = newNode->parent->parent->right;
                        if (uncleNode->color == RED) {
                            newNode->parent->color = BLACK;
                            uncleNode->color = BLACK;
                            newNode->parent->parent->color = RED;
                            newNode = newNode->parent->parent;
                        }
                        else {
                            if (newNode == newNode->parent->right) {
                                newNode = newNode->parent;
                                rotate_left(newNode);
                            }

                            newNode->parent->color = BLACK;
                            newNode->parent->parent->color = RED;
                            rotate_right(newNode->parent->parent);
                        }
                    }
                }

                root->color = BLACK;
            }
        }
        else {
            insert(node->right, key, value);
        }
    }
    else if (node->key > key) {
        if (node->left == sib<T, U>) {
            RBNode<T, U>* newNode = new RBNode<T, U>(key, value);
            node->left = newNode;
            newNode->parent = node;

            newNode->left = sib<T, U>;
            newNode->right = sib<T, U>;
            newNode->color = RED;

            if (root->parent) {
                RBNode<T, U>* uncleNode = nullptr;
                while (newNode != (root) && newNode->parent->color == RED) {
                    if (newNode->parent == newNode->parent->parent->right) {
                        uncleNode = newNode->parent->parent->left;
                        if (uncleNode->color == RED) {
                            newNode->parent->color = BLACK;
                            uncleNode->color = BLACK;
                            newNode->parent->parent->color = RED;
                            newNode = newNode->parent->parent;
                        }
                        else {
                            if (newNode == newNode->parent->left) {
                                newNode = newNode->parent;
                                rotate_right(newNode);
                            }

                            newNode->parent->color = BLACK;
                            newNode->parent->parent->color = RED;
                            rotate_left(newNode->parent->parent);
                        }
                    }
                    else {
                        uncleNode = newNode->parent->parent->right;
                        if (uncleNode->color == RED) {
                            newNode->parent->color = BLACK;
                            uncleNode->color = BLACK;
                            newNode->parent->parent->color = RED;
                            newNode = newNode->parent->parent;
                        }
                        else {
                            if (newNode == newNode->parent->right) {
                                newNode = newNode->parent;
                                rotate_left(newNode);
                            }

                            newNode->parent->color = BLACK;
                            newNode->parent->parent->color = RED;
                            rotate_right(newNode->parent->parent);
                        }
                    }
                }

                root->color = BLACK;
            }
        }
        else {
            insert(node->left, key, value);
        }
    }
    else {
        node->value = value;
    }
#endif
    return nullptr;
}

template<typename T, typename U>
U RBTree<T, U>::search(RBNode<T, U>*& node, const T& key) {
    if (node == sib<T, U>)
        return U();

    if (node->key > key) {
        return search(node->left, key);
    }
    else if (node->key < key) {
        return search(node->right, key);
    }
    else {
        return node->value;
    }
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::searchNode(RBNode<T, U>*& node, const T& key) {
    if (node == sib<T, U>)
        return nullptr;

    if (node->key > key) {
        return searchNode(node->left, key);
    }
    else if (node->key < key) {
        return searchNode(node->right, key);
    }
    else {
        return node;
    }
}

template<typename T, typename U>
RBNode<T, U>* RBTree<T, U>::remove(RBNode<T, U>*& node, const T& key) {
    RBNode<T, U>* foundNode = searchNode(node, key);
    RBNode<T, U>* removed = nullptr;
    RBNode<T, U>* successor = nullptr;

    if (foundNode == nullptr) {
        return nullptr;
    }

    if (foundNode->left == sib<T, U> || foundNode->right == sib<T, U>) {
        removed = foundNode;
    }
    else {

        RBNode<T, U>* pCur = foundNode->right;

        while (pCur->left != sib<T, U>) {
            pCur = pCur->left;
        }

        removed = pCur;
        foundNode->key = removed->key;
        foundNode->value = removed->value;
    }

    if (removed->left != sib<T, U>) {
        successor = removed->left;
    }
    else {
        successor = removed->right;
    }

    successor->parent = removed->parent;

    if (removed->parent == nullptr) {
        root = successor;
    }
    else {
        if (removed == removed->parent->left) {
            removed->parent->left = successor;
        }
        else {
            removed->parent->right = successor;
        }
    }

    if (removed->color == BLACK) {
        RBNode<T, U>* siblingNode = nullptr;
        while (successor->parent != nullptr && successor->color == BLACK) {
            if (successor == successor->parent->right) {
                siblingNode = successor->parent->left;

                if (siblingNode->color == RED) {
                    siblingNode->color = BLACK;
                    successor->parent->color = RED;
                    rotate_right(successor->parent);
                }
                else {
                    if (siblingNode->left->color == BLACK && siblingNode->right->color == BLACK) {
                        successor = successor->parent;
                        siblingNode->color = RED;
                    }
                    else {
                        if (siblingNode->right->color == RED) {
                            siblingNode->right->color = BLACK;
                            siblingNode->color = RED;

                            rotate_left(siblingNode);
                            siblingNode = successor->parent->left;
                        }

                        siblingNode->color = successor->parent->color;
                        siblingNode->left->color = BLACK;
                        successor->parent->color = BLACK;

                        rotate_right(successor->parent);
                        successor = root;
                    }
                }
            }
            else {
                siblingNode = successor->parent->right;

                if (siblingNode->color == RED) {
                    siblingNode->color = BLACK;
                    successor->parent->color = RED;
                    rotate_left(successor->parent);
                }
                else {
                    if (siblingNode->left->color == BLACK && siblingNode->right->color == BLACK) {
                        siblingNode->color = RED;
                        successor = successor->parent;
                    }
                    else {
                        if (siblingNode->left->color == RED) {
                            siblingNode->left->color = BLACK;
                            siblingNode->color = RED;

                            rotate_right(siblingNode);
                            siblingNode = successor->parent->right;
                        }

                        siblingNode->color = successor->parent->color;
                        siblingNode->right->color = BLACK;
                        successor->parent->color = BLACK;

                        rotate_left(successor->parent);
                        successor = root;
                    }
                }
            }
        }

        successor->color = BLACK;
    }

    return removed;
}

template<typename T, typename U>
void RBTree<T, U>::removeall(RBNode<T, U>*& node) {
    if (node->right != sib<T, U>)
        removeall(node->right);

    if (node->left != sib<T, U>)
        removeall(node->left);

    node->left = nullptr;
    node->right = nullptr;

    delete node;
}
