//
// Created by chris on 05/01/2026.
//

// includes
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <array>
#include <algorithm>

/* Opgave 1:
 *
 */
int sumOfDigits(const int n, int digit = 0) {
    /*Returns the sum og the individual digits*/
    static const std::string s = std::to_string(n);
    if (digit >= s.size()) {
        return 0;
    }
    const char c = s[digit];
    const int sum = atoi(&c);
    return sumOfDigits(n, ++digit) + sum;
}

/* Opgave 2:
 *
 * Store O:
 *  indre loop worst case er i = 1 og j = N
 *  dsv i vil være konstant og j og K loops vil være N
 *  der for er tidskompleksiteten O(N^2)
 *
 */

/* Opgave 3:
 * skip
 *
 */

/* Opgave 4:
 *
 *
 */
void rotate(std::vector<int>& arr, const int k) {
    if (arr.empty() || k == 0) {
        return;
    }
    if (arr.size() % 2 == 0) {
        for (int i = 0; i < k; i++) {
            std::swap(arr[i], arr[arr.size() + i - k]);
        }
    }
    else {
        const int size = arr.size();
        arr.reserve(size + size + k);
        for (int i = 0; i < size - k; i++) {
            arr.emplace_back(arr[i]);
        }
        arr.erase(arr.begin(), arr.begin() + size - k);
    }
}

void rotate_ideal(std::vector<int>& arr, const int k) {
    if (arr.empty() || k == 0 || k > arr.size()) return;
    std::ranges::rotate(arr, arr.end() - k);
}

/* Opgave 5:
 *
 * Delopgave 1:
 *
 *
 *
 * Delopgave 2:

E	TRUE 	0	0
A	TRUE 	7	E
B	TRUE 	39	C
C	TRUE 	19	A
D	TRUE 	51	C

*/

/* Opgave 5:
*
* P burde være på plads 7 ikke plads 8.
*
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
            std::cout << "Failed to place Y" << elem << " — table full or probing exhausted\n";
        }
    }

    for (int i = 0; i < table_size; ++i) {
        std::cout << i << " " << table[i] << "\n";
    }
}

int test_probing() {
    quad_prob(8, 5, 17);
}

/* Opgave 7:
 *  node 13 (med værdi 64) vil være det højre child af node 6 (med værdi 65) det er lavere
 *  og derfor kan den nedenstående tabel ikke repræsentere en prioritetskø.
 */


int main() {
    std::cout << sumOfDigits(1024) << "\n";
    std::vector<int> arr = {1,2,3,4,5,6,7};
    rotate(arr, 3);
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i] << " ";
    }

    test_probing();

    return 0;
}