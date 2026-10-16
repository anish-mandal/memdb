#pragma once

#include "cache_entry.hpp"

#include <cstddef>
#include <string>

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    LinkedList(const LinkedList&) = delete;
    LinkedList& operator=(const LinkedList&) = delete;

    void insert_or_assign(const std::string& key, const std::string& value);
    CacheEntry* find(const std::string& key);
    const CacheEntry* find(const std::string& key) const;
    bool contains(const std::string& key) const;
    bool remove(const std::string& key);

    std::size_t size() const;
    bool empty() const;
    void clear();

private:
    struct Node {
        CacheEntry entry;
        Node* next;
    };

    Node* head_;
    std::size_t entry_count_;
};