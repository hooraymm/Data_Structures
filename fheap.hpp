#ifndef __FHEAP_H_
#define __FHEAP_H_

#include <iostream>
#include <initializer_list>
#include <optional>
#include <vector>
#include <cmath>
#include <memory>
#include <queue>

template <typename T>
class PriorityQueue {
public:
    virtual void insert(const T& item) = 0;
    virtual std::optional<T> extract_min() = 0;
    virtual bool is_empty() const = 0;
};

template <typename T>
class FibonacciNode {
public:
    // Constructors
    FibonacciNode();
    FibonacciNode(const T& item)
        :key(item), degree(0), marked(false), child(nullptr), right(nullptr) {}

    // Destructor
    // You can implement your custom destructor.
    ~FibonacciNode() = default;

    T key;
    size_t degree;
    bool marked;

    std::shared_ptr<FibonacciNode<T>> right;
    std::shared_ptr<FibonacciNode<T>> child;
    // NOTE: To prevent circular reference, left/parent pointer should be set to weak_ptr.
    std::weak_ptr<FibonacciNode<T>> left;
    std::weak_ptr<FibonacciNode<T>> parent;
};

template <typename T>
class FibonacciHeap : public PriorityQueue<T> {
public:
    // Constructors
    FibonacciHeap()
        : min_node(nullptr), size_(0) {}
    FibonacciHeap(const T& item)
        : min_node(nullptr), size_(0) {
        insert(item);
    }

    // Disable copy constructor.
    FibonacciHeap(const FibonacciHeap<T>&);

    // Destructor
    ~FibonacciHeap();

    void insert(const T& item) override;
    void insert(std::shared_ptr<FibonacciNode<T>>& node);

    // Return raw pointer of the min_node.
    FibonacciNode<T>* get_min_node() { return min_node.get(); }

    std::optional<T> get_min() const;

    std::optional<T> extract_min() override;

    void decrease_key(std::shared_ptr<FibonacciNode<T>>& x, T new_key);

    void remove(std::shared_ptr<FibonacciNode<T>>& node);

    bool is_empty() const override { return !size_; }

    size_t size() const { return size_; }

private:

    std::shared_ptr<FibonacciNode<T>> min_node;
    size_t size_;

    void consolidate();
    void merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y);
    void cut(std::shared_ptr<FibonacciNode<T>>& x);
    void recursive_cut(std::shared_ptr<FibonacciNode<T>>& x);

    void detach_circular_dll(std::shared_ptr<FibonacciNode<T>>& x);

};

template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
    // TODO
    // NOTE: Be aware of memory leak or memory error.
    while (!is_empty()) {
        extract_min();
    }
}

template <typename T>
std::optional<T> FibonacciHeap<T>::get_min() const {
    if (!min_node)
        return std::nullopt;
    else
        return min_node->key;
}

template <typename T>
void FibonacciHeap<T>::insert(const T& item) {
    std::shared_ptr<FibonacciNode<T>> node = std::make_shared<FibonacciNode<T>>(item);
    insert(node);
}



template <typename T>
void FibonacciHeap<T>::insert(std::shared_ptr<FibonacciNode<T>>& node) {

    node->parent.reset();
    node->child = nullptr;
    node->left = node;
    node->right = node;
    node->degree = 0;
    node->marked = false;

    merge(min_node, node);

    if (min_node == nullptr || min_node->key > node->key)
        min_node = node;

    size_++;
}

template <typename T>
std::optional<T> FibonacciHeap<T>::extract_min() {
    // TODO
    if (min_node == nullptr)return std::nullopt;

    T key = min_node->key;

    // remove parents
    std::shared_ptr<FibonacciNode<T>> mn_child = min_node->child;
    if (mn_child != nullptr) {
        std::shared_ptr<FibonacciNode<T>> current_child = mn_child;
        do {
            current_child->parent.reset();
            current_child = current_child->right;
        } while (current_child != mn_child);
    }

    merge(mn_child, min_node);

    if (min_node == min_node->right) {
        min_node->right = nullptr;
        min_node->left.reset();
        min_node = nullptr;
    }
    else {
        auto temp = min_node;
        min_node = min_node->right;

        detach_circular_dll(temp);
        temp->right = nullptr;
        temp->left.reset();
    }

    if (min_node != nullptr)
        consolidate();

    size_--;
    return key;
}

template <typename T>
void FibonacciHeap<T>::decrease_key(std::shared_ptr<FibonacciNode<T>>& x, T new_key) {
    // TODO
    x->key = new_key;
    std::weak_ptr<FibonacciNode<T>> parent = x->parent.lock();
    if (parent != nullptr && x->key < parent->key) {
        cut(x);
        recursive_cut(parent);
    }
}

template <typename T>
void FibonacciHeap<T>::remove(std::shared_ptr<FibonacciNode<T>>& x) {

    decrease_key(x, std::numeric_limits<T>::min());
    extract_min();
}

template <typename T>
void FibonacciHeap<T>::consolidate() {
    float phi = (1 + sqrt(5)) / 2.0;
    int len = int(log(size_) / log(phi)) + 10;
    // TODO

    std::vector<std::shared_ptr<FibonacciNode<T>>> A(len, nullptr);
    std::vector<std::shared_ptr<FibonacciNode<T>>> nodes;

    auto temp = min_node;
    do {
        nodes.push_back(temp);
        temp = temp->right;
    } while (temp != min_node);


    for (auto node : nodes) {
        int degree = node->degree;

        detach_circular_dll(node);

        while (A[degree] != nullptr) {
            auto same_degree_node = A[degree];
            if (same_degree_node->key < node->key)
                std::swap(node, same_degree_node);


            // make node as parent node of same_degree_node
            same_degree_node->parent = node;
            merge(node->child, same_degree_node);
            node->child = same_degree_node;
            node->degree++;
            same_degree_node->marked = false;

            A[degree] = nullptr;
            degree++;

        }
        A[node->degree] = node;
        min_node = node;
    }

    for (int i = 0; i < len; i++) {
        if (A[i] != nullptr && A[i] != min_node)
            merge(A[i], min_node);
    }

    auto node = min_node;
    auto start = min_node;

    do {
        if (min_node->key > node->key) {
            min_node = node;
        }
        node = node->right;
    } while (node != start);

}

template <typename T>
void FibonacciHeap<T>::detach_circular_dll(std::shared_ptr<FibonacciNode<T>>& x) {
    if (x != nullptr && x->right != x) {
        std::weak_ptr<FibonacciNode<T>> left = x->left;
        std::shared_ptr<FibonacciNode<T>> right = x->right;

        left.lock()->right = right;
        right->left = left;

        x->left = x->right = x;
    }
}

template <typename T>
void FibonacciHeap<T>::merge(std::shared_ptr<FibonacciNode<T>>& x, std::shared_ptr<FibonacciNode<T>>& y) {
    // TODO
    if (x == nullptr || y == nullptr)return;

    std::shared_ptr<FibonacciNode<T>> a = x->right;
    std::weak_ptr<FibonacciNode<T>> b = y->left;

    x->right = y;
    y->left = x;

    a->left = b;
    b.lock()->right = a;
}

template <typename T>
void FibonacciHeap<T>::cut(std::shared_ptr<FibonacciNode<T>>& x) {
    // TODO
    std::weak_ptr<FibonacciNode<T>> parent = x->parent.lock();
    if (parent == nullptr)return;

    if (x == x->right)
        parent.lock()->child = nullptr;
    else
        parent.lock()->child = x->right;

    detach_circular_dll(x); 
    parent.lock()->degree--; 
    merge(min_node, x); 
    x->parent.reset();
    x->marked = false; 
}

template <typename T>
void FibonacciHeap<T>::recursive_cut(std::shared_ptr<FibonacciNode<T>>& x) {
    // TODO
    if (x == nullptr)return;
    std::weak_ptr<FibonacciNode<T>> parent = x->parent.lock();
    if (parent == nullptr)return;
    if (x->marked == false)
        x->marked = true;
    else {
        cut(x);
        recursive_cut(parent);
    }

}

#endif // __FHEAP_H_
