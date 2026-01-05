//
// Created by chris on 04/01/2026.
//

#include <string>
#include <string_view>
#include <iostream>
#include <ostream>
#include <fstream>
#include <cmath>
#include <memory>
#include <utility>
#include <vector>
#include <array>
#include <algorithm>

// Helper functions
std::string read_file(const std::string_view filename) {
    // Reads the files content
    std::ifstream ifs((filename.data()));
    const std::string content( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()    ) );
    return content;
}


// Opgave 1:
int countLettersInWord(const std::string_view word, const char letter, const int index = 0) {
    if (word.length() <= index) {
        return 0;
    }

    if (word[index] == letter) {
        return countLettersInWord(word, letter, index + 1) + 1;
    }
    return countLettersInWord(word, letter, index + 1);
}

/* opgave 2:

    inner loop: O(sqrt(N))
    midder loop: O((log(n))^2)
    upper loop: O(N)

    second loop: O(N*sqrt(N))

 big O will be O(N*sqrt(N)) since when N gets really large, this has the most to say
    when N is larger than ~306 the Y becomes larger than X and therefore the second loop is the dominant one
*/

std::pair<long, long> bigOh(const double N)
{
    long x = 0; long y = 0;
    for (int i = 0; i < N; i++) //for-loop nummer 1
    {
        for (int j = 0; j < std::pow(std::log(N),2); j++)
            // log er ln (den naturlige logaritme)
        {
            for (int k = 0; k <= std::sqrt(N); k++)
            {
                x++;
            }
        }
        i += i;
    }

    for (long k = 0; k < N*std::sqrt(N); k++) //for-loop nummer 2
        y++;

    return std::pair(x, y);
}

void test_bigOh() {
    int x{0}, y{1};
    int n = 1;
    std::vector<std::pair<int, int>> var;
    var.reserve(500);
    while (n < 500) {
        std::pair<int, int> xy = bigOh(n);
        x = xy.first;
        y = xy.second;
        int diff = x - y;
        var.emplace_back(n, diff);
        n++;
    }

    for (int i = 0; i < var.size(); i++) {
        std::cout << var[i].first << " " << var[i].second << std::endl;
    }
}

/*
 Opgave 4:

 */
struct BinaryNode {
    std::shared_ptr<BinaryNode> left;
    std::shared_ptr<BinaryNode> right;
    int val;
    BinaryNode(const int value): left(nullptr), right(nullptr), val(value) {}
};

bool findRouteImpl(const std::shared_ptr<BinaryNode> &node, const int value, std::string &out) {
    if (!node) return false;
    if (node->val == value) {
        out = std::to_string(node->val) + " ";
        return true;
    }
    std::string sub;
    if (findRouteImpl(node->left, value, sub) || findRouteImpl(node->right, value, sub)) {
        out = std::to_string(node->val) + " " + sub;
        return true;
    }
    return false;
}

std::string findRoute(const std::shared_ptr<BinaryNode> &root, const int value) {
    std::string path;
    if (findRouteImpl(root, value, path)) return path;
    return "NOT_FOUND"; // not found
}


void test_findRoute() {
    const auto root = std::make_shared<BinaryNode>(45);
    root->left = std::make_shared<BinaryNode>(15);
    root->right = std::make_shared<BinaryNode>(79);
    // Making left side
    root->left->left = std::make_shared<BinaryNode>(10);
    root->left->right = std::make_shared<BinaryNode>(20);
    root->left->left->right = std::make_shared<BinaryNode>(12);
    // Making right side
    root->right->left = std::make_shared<BinaryNode>(55);
    root->right->right = std::make_shared<BinaryNode>(90);
     root->right->left->left = std::make_shared<BinaryNode>(50);
    std::cout << findRoute(root, 10) << "\n";
    std::cout << findRoute(root, 50) << "\n";
}

/* Opgave 5

F	TRUE	0	0
A	TRUE	15	B
B	TRUE	14	D
C	TRUE	16	A
D	TRUE	5	E
E	TRUE	4	F
G	TRUE	34	J
H	TRUE	21	C
I	TRUE	19	A
J	TRUE	24	I

 */

/* Opgave 6

0 Y6
1
2 Y5
3 X
4 Y1
5
6 Y4
7 Y2
8
9
10
11
12 Y3

 */

void quad_prob(const int amount_elements, const int hashing_index, const int table_size) {
    if (table_size <= 0) return;
    const int start = ((hashing_index % table_size) + table_size) % table_size;
    std::vector<std::string> table(table_size);

    table[start] = "X";

    for (int elem = 0; elem < amount_elements; ++elem) {
        bool placed = false;
        for (int probe = 0; probe < table_size; ++probe) {
            const int idx = (start + probe * probe) % table_size;
            if (table[idx].empty()) {
                table[idx] = "Y" + std::to_string(elem);
                placed = true;
                break;
            }
            std::cout << "Collision for Y" << elem << " at index " << idx << "\n";
        }
        if (!placed) {
            std::cout << "Failed to place Y" << elem << " - table full or probing exhausted\n";
        }
    }

    for (int i = 0; i < table_size; ++i) {
        std::cout << i << " " << table[i] << "\n";
    }
}

/* Opgave 7
    Because 13 will become the right child of 28 and should be larger to be its child, but its 27.
    Therefore it cannot be represented as a priority queue.

 */

int main() {
    std::cout << "Amount of a's in banana is: "<< countLettersInWord("banana", 'a') << std::endl;
    test_bigOh();
    test_findRoute();
    quad_prob(7, 3, 13);
}