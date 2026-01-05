// cpp
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <ranges>
#include <cfenv>
#include <cmath>
#include "CuckooHashing.h"
#include "BinaryTree.h"

bool anagrams(const std::string_view s1, const std::string_view s2) {
    if (s1.size() != s2.size()) return false;
    bool is_anagram = true;
    bool found = false;
    for (const auto s: s1) {
        for (const auto c : s2) {
            if (s == c) {
                found = true;
                break;
            }
        }
        if (!found) {
            found = false;
            is_anagram = false;
            break;
        }
    }
    return is_anagram;
}

bool anagrams_bucket_sort(const std::string_view s1, const std::string_view s2) {
    if (s1.size() != s2.size()) return false;
    std::vector<int> bucket(128, 0);
    for (int i = 0; i < s1.size(); ++i) {
        ++bucket[static_cast<int>(s1[i])];
        --bucket[static_cast<int>(s2[i])];
    }

    if (std::ranges::all_of(bucket, [](const int x){ return x == 0; })) {
        return true;
    }

    return true;
}

bool quad_calculate_x(const std::span<int> data, int x) {
    for (const auto outer : data) {
        for (const auto inner: data) {
            if (outer + inner == x) {
                return true;
            }
        }
    }
    return false;
}

double pythagorean_triplet() {
    /* This could either be done with 3 for loops or we could improve it a bit by using only two loops and compute the c value */
    constexpr double target_sum = 1000.0;
    for (int a = 1; a < target_sum; ++a) {
        for (int b = a + 1; b < target_sum; ++b) {
            const int c = static_cast<int>(std::sqrt(static_cast<float>(a * a + b * b)));
            if (a + b + c == target_sum ) {
                std::cout << "Found Pythagorean triplet: " << a << ", " << b << ", " << c << std::endl;
                return a * b * c;
            }
        }
    }
    return 0.0;
}

int getNumberEqual(const std::vector<int>& x, int n, int val) {
    if (n < 0) {
        return 0;
    }

    if (x[n] == val) {
        return getNumberEqual(x, n-1, val) + 1;
    }

    return getNumberEqual(x, n-1, val);
}

bool lin_calculate_x(const std::span<int> data, int x) {
    int tail = 0;
    int head = static_cast<int>(data.size()) - 1;
    int result = 0;
    while (tail < head) {
        result = data[tail]  + data[head];
        if (result == x) {
            return true;
        }
        if (result < x){
            tail++;
        }
        if (result > x) {
            head--;
        }
    }

    return false;
}


void create_perf_rec_int(BinaryTree<int>& tree, const int medium, const int multiplier) {
    if (multiplier <= 0) {
        tree.insert(medium); // create the leaf nodes
        return;
    }
    tree.insert(medium);
    create_perf_rec_int(tree, medium - multiplier, multiplier / 2);
    create_perf_rec_int(tree, medium + multiplier, multiplier / 2);

}

void create_perf_rec_str(BinaryTree<std::string> &tree, const char medium, const int multiplier) {
    std::cout << "Inserting node: " << medium << " with multiplier " << multiplier << std::endl;
    if (multiplier <= 0) {
        tree.insert(std::string(1, medium)); // create the leaf nodes
        return;
    }
    tree.insert(std::string(1, medium));
    create_perf_rec_str(tree, medium - multiplier, multiplier / 2);
    create_perf_rec_str(tree, medium + multiplier, multiplier / 2);

}

BinaryTree<std::string> create_perfect_binary_tree_str(const int height) {
    BinaryTree<std::string> perf_tree;
    const int offset = (1 << height);  // 2^height
    const char medium = 'a' + offset - 1;  // Root character
    create_perf_rec_str(perf_tree, medium, offset / 2);
    return perf_tree;
}

BinaryTree<int> create_perfect_binary_tree_int(const int height) {
    BinaryTree<int> perf_tree;
    const int amount = std::pow(2, height + 1) - 1;
    const int medium = std::round(amount / 2) + (amount % 2);
    create_perf_rec_int(perf_tree, medium, medium/2);
    return perf_tree;
}

template<class T>
BinaryTree<T> create_perfect_binary_tree(const int height) {
    BinaryTree<T> perf_tree;
    if constexpr (std::is_integral_v<T>) {
        perf_tree = create_perfect_binary_tree_int(height);
    }
    else {
        perf_tree = create_perfect_binary_tree_str(height);
    }
    return perf_tree;
}


void test_binary_tree() {
    const BinaryTree<int> perf_tree = create_perfect_binary_tree<int>(3);
    perf_tree.print();
    return;

    BinaryTree<int> tree;
    tree.insert(100);
    tree.insert(90);
    tree.insert(110);
    tree.insert(80);
    tree.insert(95);
    tree.insert(105);
    tree.insert(120);
    tree.insert(70);
    tree.insert(85);
    tree.insert(92);
    tree.insert(97);
    tree.insert(103);
    tree.insert(108);
    tree.insert(111);
    tree.insert(130);
    tree.remove(105);
    tree.insert(83);
    tree.remove(130);
    tree.insert(60);
    tree.remove(95);
    tree.print();


    // tree.insert(8);
    // tree.insert(5);
    // tree.insert(10);
    // tree.insert(2);
    // tree.insert(6);
    // tree.insert(9);
    // tree.insert(11);
    // tree.insert(3);
    // tree.insert(1);
    // // tree.insert(7);
    // // tree.insert(4);
    // // tree.insert(12);
    // // tree.insert(13);


    int length = tree.internal_path_length();
    std::cout << "Length of internal path = " << length << std::endl;

    std::vector<int> pre_order;
    tree.pre_order_traversal(pre_order);
    std::cout << "Pre-order traversal: ";
    for (const auto &val : pre_order) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    std::vector<int> in_order;
    tree.in_order_traversal(in_order);
    std::cout << "In-order traversal: ";
    for (const auto &val : in_order) {
        std::cout << val << ' ';
    }
    std::cout << '\n';

    std::vector<std::vector<int>> level_order;
    tree.level_order_traversal(level_order);
    std::cout << "Level-order traversal: ";
    for (const auto &level : level_order) {
        std::cout << " [";
        for (const auto &val : level) {
            std::cout << ' ' << val << ' ';
        }
        std::cout << "] ";
    }
    std::cout << '\n';


    std::cout << "Binary Tree contents (in-order): \n";
    int search_val = 4;
    if (tree.contains(search_val)) {
        std::cout << "Tree contains " << search_val << '\n';
    } else {
        std::cout << "Tree does not contain " << search_val << '\n';
    }

    search_val = 10;
    if (tree.contains(search_val)) {
        std::cout << "Tree contains " << search_val << '\n';
    } else {
        std::cout << "Tree does not contain " << search_val << '\n';
    }
}

void test_calculate_x() {
    std::vector<int> data = {1,2,3,4,5,6,7,8,9,10};
    int x = 3;

    if (quad_calculate_x(data, x)) {
        std::cout << "Found x=" << x << " using quadratic method.\n";
    } else {
        std::cout << "Did not find x=" << x << " using quadratic method.\n";
    }

    if (lin_calculate_x(data, x)) {
        std::cout << "Found x=" << x << " using linear method.\n";
    } else {
        std::cout << "Did not find x=" << x << " using linear method.\n";
    }
}

void test_anagrams() {
    std::string str1 = "listen";
    std::string str2 = "silent";
    std::string str3 = "hello";

    std::string str4 = "stale";
    std::string str5 = "least";

    if (anagrams_bucket_sort(str1, str2)) {
        std::cout << str1 << " and " << str2 << " are anagrams.\n";
    } else {
        std::cout << str1 << " and " << str2 << " are not anagrams.\n";
    }

    if (anagrams_bucket_sort(str1, str3)) {
        std::cout << str1 << " and " << str3 << " are anagrams.\n";
    } else {
        std::cout << str1 << " and " << str3 << " are not anagrams.\n";
    }

    if (anagrams_bucket_sort(str4, str5)) {
        std::cout << str4 << " and " << str5 << " are anagrams.\n";
    } else {
        std::cout << str4 << " and " << str5 << " are not anagrams.\n";
    }

}

void test_cuckoo_hashing() {
    // start with a reasonably sized table to avoid immediate heavy rehashing
    CuckooHashing<std::string> cuckoo(257);

    // a set of short prefixes to generate many distinct strings
    std::vector<std::string> prefixes = {
        "alpha","beta","gamma","delta","epsilon",
        "zeta","eta","theta","iota","kappa",
        "lambda","mu","nu","xi","omicron",
        "pi","rho","sigma","tau","upsilon",
        "phi","chi","psi","omega","sky",
        "tree","rock","blue","red","green"
    };

    // generate strings like alpha0, alpha1, ... beta0, ...
    std::vector<std::string> items;
    for (const auto &p : prefixes) {
        for (int i = 0; i < 10; ++i) {
            items.push_back(p + std::to_string(i));
        }
    }

    // add some extra readable phrases
    std::vector<std::string> extras = {
        "hello", "world", "example", "test", "cuckoo",
        "collision", "insert", "remove", "rehash", "probe"
    };
    items.insert(items.end(), extras.begin(), extras.end());

    // shuffle insertion order to exercise cuckoo behavior
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(items.begin(), items.end(), gen);

    // insert and report failures
    int inserted = 0;
    for (const auto &s : items) {
        if (cuckoo.insert(s)) ++inserted;
        else std::cout << "Failed to insert: " << s << '\n';
    }

    std::cout << "Inserted " << inserted << " of " << items.size() << " items.\n\n";
    cuckoo.print_tables();
}

int main() {
    // test_binary_tree();
    std::cout << "sum og pythagoreon triplet: " << static_cast<int>(pythagorean_triplet()) << "\n";
    std::cout << "Number of equal to 4: " << getNumberEqual(std::vector<int>{7,4,1,3,5,6,4,8}, 8, 4) << "\n";
    return 0;
}