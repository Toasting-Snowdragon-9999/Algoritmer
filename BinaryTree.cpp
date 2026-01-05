//
// Created by chris on 06/12/2025.
//

#include "BinaryTree.h"

#define ALLOWED_IMBALANCE 1

template<class T>
void BinaryTree<T>::insert_impl(const T &value, std::unique_ptr<BinaryNode<T>> &node) {
    if (node == nullptr) {
        node = std::make_unique<BinaryNode<T>>(value);
        return;
    }

    if (value < node->val) {
         insert_impl(value, node->left);
    }
    else if (value > node->val) {
         insert_impl(value, node->right);
    }
    else {

    }
    balance_tree(node);
}

template<class T>
int BinaryTree<T>::height(const std::unique_ptr<BinaryNode<T>> &node) const {
    if (node == nullptr) {
        return 0;
    }
    int left_height = height(node->left);
    int right_height = height(node->right);
    return std::max(left_height, right_height) + 1;
}

template<class T>
void BinaryTree<T>::balance_tree(std::unique_ptr<BinaryNode<T>> &node) {
    if (node == nullptr) {
        return;
    }

    if (height(node->left) - height(node->right) > ALLOWED_IMBALANCE) {
        std::cout << "Balancing node with value: " << node->val << std::endl;

        // Left heavy
        if (height(node->left->left) >= height(node->left->right)) {
            rotate_with_left_child(node);
        } else {
            double_with_left_child(node);
        }
    } else if (height(node->right) - height(node->left) > ALLOWED_IMBALANCE) {
        std::cout << "Balancing node with value: " << node->val << std::endl;

        // Right heavy
        if (height(node->right->right) >= height(node->right->left)) {
            rotate_with_right_child(node);
        } else {
            double_with_right_child(node);
        }

    }
}

template<class T>
void BinaryTree<T>::rotate_with_left_child(std::unique_ptr<BinaryNode<T>> &node) {
    auto tmp = std::move(node->left);
    node->left = std::move(tmp->right);
    tmp->right = std::move(node);
    node = std::move(tmp);
}

template<class T>
void BinaryTree<T>::double_with_left_child(std::unique_ptr<BinaryNode<T>> &node) {
    rotate_with_right_child(node->left);
    rotate_with_left_child(node);
}

template<class T>
void BinaryTree<T>::rotate_with_right_child(std::unique_ptr<BinaryNode<T>> &node) {
    auto tmp = std::move(node->right);
    node->right = std::move(tmp->left);
    tmp->left = std::move(node);
    node = std::move(tmp);
}

template<class T>
void BinaryTree<T>::double_with_right_child(std::unique_ptr<BinaryNode<T>> &node){
    rotate_with_left_child(node->right);
    rotate_with_right_child(node);
}

template<class T>
bool BinaryTree<T>::contains_impl(const T &value, const std::unique_ptr<BinaryNode<T>> &node) const {
    if (node == nullptr) {
        return false;
    }

    if (value < node->val) {
        return contains_impl(value, node->left);
    }

    if (value > node->val) {
        return contains_impl(value, node->right);
    }

    return true;
}

template<class T>
BinaryNode<T>* BinaryTree<T>::find_min_impl(const std::unique_ptr<BinaryNode<T>> &node) const {
    // To improve efficiency dont use recursion.
    if (!root_) {
        throw std::runtime_error("Tree is empty");
        return nullptr;
    }
    if (node->left == nullptr) {
        return node.get();
    }
    return find_min_impl(node->left);
}

template<class T>
T BinaryTree<T>::find_max_impl(const std::unique_ptr<BinaryNode<T>> &node) const {
    // To improve efficiency dont use recursion.
    if (!root_) {
        throw std::runtime_error("Tree is empty");
        return T{};
    }
    if (node->right == nullptr) {
        return node->val;
    }
    return find_max_impl(node->right);

}

template<class T>
int BinaryTree<T>::max_depth() const {
    int cnt{}, cnt2{};
    bool looking_left = true;
    BinaryNode<T>* current = root_.get();
    while (current != nullptr) {
        if (current->left != nullptr && looking_left) {
            current = current->left.get();
            cnt++;
        }
        else if (current->left == nullptr && looking_left){
            looking_left = false;
            current = root_.get();
        }
        else if (current->right != nullptr) {
                current = current->right.get();
                cnt2++;
            }
        else {
            break;
        }
        }


    return cnt < cnt2 ? cnt2 : cnt;
}

template <class T>
void BinaryTree<T>::internal_path_length_impl(const std::unique_ptr<BinaryNode<T>> &node, int &cnt, int depth) const {
    int tmp = depth; // To prevent double incrementation of depth in both branches

    if (node == nullptr) {
        return;
    }
    if (node->left != nullptr) {
        cnt += ++depth;
        internal_path_length_impl(node->left, cnt, depth);
    }
    if (node->right != nullptr) {
        cnt += ++tmp;
        internal_path_length_impl(node->right, cnt, tmp);
    }
}

template<class T>
bool BinaryTree<T>::remove_impl(const T &value, std::unique_ptr<BinaryNode<T>> &node) {
    if (node == nullptr) {
        return false;
    }
    if (value < node->val) {
        return remove_impl(value, node->left);
    }
    if (value > node->val) {
        return remove_impl(value, node->right);
    }
    if (node->left && node->right) {
        // Node with two children: Get the inorder successor (smallest in the right subtree)
        node->val = find_min_impl(node->right)->val;
        // Delete the inorder successor
        return remove_impl(node->val, node->right);
    }
    // Node with one child or no child
    node = std::move((node->left) ? node->left : node->right);
    return true;
}

template<class T>
void BinaryTree<T>::pre_order_traversal_impl(std::vector<T>& res, const std::unique_ptr<BinaryNode<T>> &node) const {
    if (node == nullptr) {
        return;
    }
    res.push_back(node->val);
    pre_order_traversal_impl(res, node->left);
    pre_order_traversal_impl(res, node->right);
}

template<class T>
void BinaryTree<T>::in_order_traversal_impl(std::vector<T>& res, const std::unique_ptr<BinaryNode<T>> &node) const {
    if (node == nullptr) {
        return;
    }
    in_order_traversal_impl(res, node->left);
    res.push_back(node->val);
    in_order_traversal_impl(res, node->right);
}

template<class T>
void BinaryTree<T>::post_order_traversal_impl(std::vector<T>& res, const std::unique_ptr<BinaryNode<T>> &node) const {
    if (node == nullptr) {
        return;
    }
    post_order_traversal_impl(res, node->left);
    post_order_traversal_impl(res, node->right);
    res.push_back(node->val);
}

template<class T>
void BinaryTree<T>::level_order_traversal_impl(std::vector<std::vector<T>>& res, int level, const std::unique_ptr<BinaryNode<T>> &node) const {
    if (node == nullptr) {
        return;
    }
    if (res.size() <= level) {
        res.push_back(std::vector<T>{});
    }
    res[level].push_back(node->val);
    level_order_traversal_impl(res, level + 1, node->left);
    level_order_traversal_impl(res, level + 1, node->right);
}

template<class T>
void BinaryTree<T>::print() const {
    if (!root_) {
        std::cout << "(empty tree)\n";
        return;
    }

    std::function<void(const BinaryNode<T>*, const std::string&, bool)> print_tree =
        [&](const BinaryNode<T>* node, const std::string& prefix, bool is_left) {
            if (!node) return;

            std::cout << prefix;
            std::cout << (is_left ? "|-- " : "`-- ");
            std::cout << node->val << '\n';

            if (node->left || node->right) {
                if (node->left) {
                    print_tree(node->left.get(), prefix + (is_left ? "|   " : "    "), true);
                } else {
                    std::cout << prefix << (is_left ? "|   " : "    ") << "|-- (null)\n";
                }

                if (node->right) {
                    print_tree(node->right.get(), prefix + (is_left ? "|   " : "    "), false);
                } else {
                    std::cout << prefix << (is_left ? "|   " : "    ") << "`-- (null)\n";
                }
            }
    };

    std::cout << root_->val << '\n';
    if (root_->left) {
        print_tree(root_->left.get(), "", true);
    } else if (root_->right) {
        std::cout << "|-- (null)\n";
    }

    if (root_->right) {
        print_tree(root_->right.get(), "", false);
    } else if (root_->left) {
        std::cout << "`-- (null)\n";
    }
}

template class BinaryTree<int>;
template class BinaryTree<std::string>;
