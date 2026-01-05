//
// Created by chris on 06/12/2025.
//

#ifndef ALGORITMER_CUCKOOHASHING_H
#define ALGORITMER_CUCKOOHASHING_H
#include <string_view>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <cstdint>

#define MAX_REHASH_ATTEMPTS 50

template<typename T>
class CuckooHashing {
public:
    explicit CuckooHashing(const std::size_t size = 0) : table_size_(size) {
        if (table1_.size() != table_size_) {
            table1_.resize(table_size_, T{});
        }

        if (table2_.size() != table_size_) {
            table2_.resize(table_size_, T{});
        }
    }

    ~CuckooHashing() = default;

    bool insert(T key) {
        if (table_size_ == 0) {
            return false;
        }

        if (initial_hash_function(key)) {
            return true;
        }
        if (second_hash_function(key)) {
            return true;
        }
        if (rehash_idx >= MAX_REHASH_ATTEMPTS) {
            std::cout << "Max rehash attempts reached (" << MAX_REHASH_ATTEMPTS << "). Insertion failed for key: " << key << "\n";
            return false;
        }

        return rehash();
     }

    void print_tables() const {
        if (table_size_ == 0) {
            std::cout << "Tables are empty (size 0)\n";
            return;
        }

        // convert entries to strings (empty string for sentinel T{})
        auto to_str = [](const T &v) {
            std::ostringstream oss;
            oss << v;
            return oss.str();
        };

        std::vector<std::string> s1(table_size_), s2(table_size_);
        for (std::size_t i = 0; i < table_size_; ++i) {
            if (table1_.at(i) != T{}) s1[i] = to_str(table1_.at(i));
            if (table2_.at(i) != T{}) s2[i] = to_str(table2_.at(i));
        }

        // compute column widths
        const std::string hdrIdx = "Index";
        const std::string hdr1 = "Table 1";
        const std::string hdr2 = "Table 2";
        const std::size_t idxW = std::max(hdrIdx.size(), std::to_string(table_size_ ? table_size_ - 1 : 0).size());
        std::size_t col1W = hdr1.size();
        std::size_t col2W = hdr2.size();
        for (std::size_t i = 0; i < table_size_; ++i) {
            col1W = std::max(col1W, s1[i].size());
            col2W = std::max(col2W, s2[i].size());
        }

        auto print_sep = [&]() {
            std::cout << '+' << std::string(idxW + 2, '-')
                      << '+' << std::string(col1W + 2, '-')
                      << '+' << std::string(col2W + 2, '-') << "+\n";
        };

        print_sep();
        // header
        std::cout << "| " << std::setw(idxW) << std::left << hdrIdx << " "
                  << "| " << std::setw(col1W) << std::left << hdr1 << " "
                  << "| " << std::setw(col2W) << std::left << hdr2 << " |\n";
        print_sep();

        // rows
        for (std::size_t i = 0; i < table_size_; ++i) {
            const std::string &v1 = s1[i].empty() ? "-" : s1[i];
            const std::string &v2 = s2[i].empty() ? "-" : s2[i];
            std::cout << "| " << std::setw(idxW) << std::right << i << " "
                      << "| " << std::setw(col1W) << std::left << v1 << " "
                      << "| " << std::setw(col2W) << std::left << v2 << " |\n";
        }
        print_sep();
    }

private:
    std::vector<T> table1_, table2_;
    std::size_t table_size_;
    int rehash_idx = 0;
    bool initial_hash_function(const T &key) {
        if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>) {
            return string_hash(key);
        } else if constexpr (std::is_integral_v<T>) {
            return int_hash(key);
        } else {
            return false;
        }
    }

    bool second_hash_function(const T &key) {
        if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, std::string_view>) {
            return string_hash2(key);
        } else if constexpr (std::is_integral_v<T>) {
            return int_hash2(key);
        } else {
            return false;
        }
    }

    bool string_hash(const T &key) {
        int hash_val = 0;
        for (auto s: key) {
            const char c = static_cast<char>(s);
            hash_val += 37 * hash_val + c;
        }
        hash_val %= table_size_;

        if (hash_val < 0) {
            hash_val += table_size_;
        }

        if (table1_.at(hash_val) == T{}) {
            table1_[hash_val] = key;
            return true;
        }

        return false;
    }

    bool string_hash2(const T &key) {
        // Uses the Jenkins hash function (one_at_a_time)
        uint32_t hash_val = 0;
        for (auto s: key ) {
            hash_val += s;
            hash_val += hash_val << 10;
            hash_val += hash_val >> 6;
        }
        hash_val += hash_val << 3;
        hash_val ^= hash_val >> 11;
        hash_val += hash_val << 15;
        const std::size_t idx = static_cast<std::size_t>(hash_val % static_cast<uint32_t>(table_size_));

        if (table2_.at(idx) == T{}) {
            table2_[idx] = key;
            return true;
        }
        return false;
    }

    bool int_hash(const T &key) {
        static_assert(std::is_integral_v<T>, "int_hash2 requires an integral T");
        T x = key;
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = ((x >> 16) ^ x) * 0x45d9f3bu;
        x = (x >> 16) ^ x;
        x &= table_size_;
        if (table1_.at(x) != 0) {
            T temp = table1_.at(x);
            table1_[x] = key;
            return int_hash2(temp);
        }
        table1_[x] = key;
        return true;
    }

    bool int_hash2(const T &key) {
        static_assert(std::is_integral_v<T>, "int_hash2 requires an integral T");

        return false;
    }

    bool rehash() {
        std::vector<T> tmp(table1_);
        std::vector<T> tmp2(table2_);
        table_size_ *= 2;
        table1_.clear();
        table2_.clear();
        table1_.resize(table_size_, T{});
        table2_.resize(table_size_, T{});
        bool all_success = true;
        for (const auto &item : tmp2) {
            if (item != T{}) {
                if (!insert(item)) {
                    all_success = false;
                };
            }
        }
        for (const auto &item : tmp) {
            if (item != T{}) {
                if (!insert(item)) {
                    all_success = false;
                };
            }
        }
        rehash_idx++;
        return all_success;
    }

};

#endif //ALGORITMER_CUCKOOHASHING_H