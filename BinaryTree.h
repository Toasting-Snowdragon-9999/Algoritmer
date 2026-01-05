//
// Created by chris on 06/12/2025.
//

#ifndef ALGORITMER_BINARYTREE_H
#define ALGORITMER_BINARYTREE_H

#include <memory>
#include <vector>
#include <iostream>
#include <functional>

template<class T>
struct BinaryNode {
    T val;
    std::unique_ptr<BinaryNode> left;
    std::unique_ptr<BinaryNode> right;
    explicit BinaryNode(const T& x) : val(x), left(nullptr), right(nullptr) {}
    explicit BinaryNode(const T& x, std::unique_ptr<BinaryNode> left, std::unique_ptr<BinaryNode> right) : val(x), left(std::move(left)), right(std::move(right)) {}
};

template<class T>
class BinaryTree {
public:
    BinaryTree() : root_(nullptr) {}
    void insert(const T &value){insert_impl (value, root_);}
    bool remove(const T &value){return remove_impl (value, root_);}
    bool contains(const T &value) const { return contains_impl (value, root_); }
    BinaryNode<T>* find_min() const{return find_min_impl(root_);}
    T find_max() const{return find_max_impl(root_);}
    void print() const;
    void pre_order_traversal(std::vector<T>& res) const{pre_order_traversal_impl(res, root_);}
    void in_order_traversal(std::vector<T>& res) const{in_order_traversal_impl(res, root_);}
    void post_order_traversal(std::vector<T>& res) const{post_order_traversal_impl(res, root_);}
    void level_order_traversal(std::vector<std::vector<T>>& res) const{level_order_traversal_impl(res, 0, root_);}

    [[nodiscard]] int max_depth() const;
    [[nodiscard]] int internal_path_length() const{int cnt = 0; internal_path_length_impl(root_, cnt); return cnt;}

private:
    void insert_impl(const T &value, std::unique_ptr<BinaryNode<T>> &node );
    bool remove_impl(const T &value, std::unique_ptr<BinaryNode<T>> &node);
    bool contains_impl(const T &value, const std::unique_ptr<BinaryNode<T>> &node) const;
    void pre_order_traversal_impl(std::vector<T>& res, const std::unique_ptr<BinaryNode<T>> &node) const;
    void in_order_traversal_impl(std::vector<T>& res, const std::unique_ptr<BinaryNode<T>> &node) const;
    void post_order_traversal_impl(std::vector<T>& res, const std::unique_ptr<BinaryNode<T>> &node) const;
    void level_order_traversal_impl(std::vector<std::vector<T>>& res, int level, const std::unique_ptr<BinaryNode<T>> &node) const;

    void internal_path_length_impl(const std::unique_ptr<BinaryNode<T>> &node, int &cnt, int depth =0) const;

    BinaryNode<T>* find_min_impl(const std::unique_ptr<BinaryNode<T>> &node) const;
    T find_max_impl(const std::unique_ptr<BinaryNode<T>> &node) const;
    int height(const std::unique_ptr<BinaryNode<T>> &node) const;

    void rotate_with_left_child(std::unique_ptr<BinaryNode<T>> &node);

    void double_with_left_child(std::unique_ptr<BinaryNode<T>> &node);

    void rotate_with_right_child(std::unique_ptr<BinaryNode<T>> &node);

    void double_with_right_child(std::unique_ptr<BinaryNode<T>> &node);

    void balance_tree(std::unique_ptr<BinaryNode<T>> &node);


    std::unique_ptr<BinaryNode<T>> root_;

};
#endif //ALGORITMER_BINARYTREE_H