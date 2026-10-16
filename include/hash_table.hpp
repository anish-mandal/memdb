#pragma once

#include "linked_list.hpp"

#include <cstddef>
#include <string>
#include <vector>

class HashTable {
public:
    explicit HashTable(std::size_t bucket_count = 16);

    void insert_or_assign(const std::string& key, const std::string& value);
    CacheEntry* find(const std::string& key);
    const CacheEntry* find(const std::string& key) const;
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);

    std::size_t size() const;
    bool empty() const;
    void clear();

private:
    std::size_t bucket_index(const std::string& key) const;

    std::vector<LinkedList> buckets_;
    std::size_t entry_count_;
};