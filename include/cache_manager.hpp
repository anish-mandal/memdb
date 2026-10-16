#pragma once

#include "eviction_policy.hpp"
#include "hash_table.hpp"
#include "min_heap.hpp"
#include <chrono>
#include <cstddef>
#include <memory>
#include <optional>
#include <string>

enum class EvictionPolicyType { LRU, FIFO };

class CacheManager {
public:
    explicit CacheManager(std::size_t capacity,
                           EvictionPolicyType policy = EvictionPolicyType::LRU);
    CacheManager(std::size_t capacity, std::unique_ptr<EvictionPolicy> policy);

    bool set(const std::string& key, const std::string& value);
    bool set(const std::string& key, const std::string& value,
             std::chrono::milliseconds ttl);
    std::optional<std::string> get(const std::string& key);
    bool get(const std::string& key, std::string& value);
    bool contains(const std::string& key);
    bool remove(const std::string& key);
    void clear();
    std::size_t size();
    bool empty() { return size() == 0; }
    std::size_t capacity() const { return capacity_; }
    std::size_t purge_expired();

private:
    void expire_entries();
    void evict_if_needed();
    HashTable table_;
    std::unique_ptr<EvictionPolicy> policy_;
    MinHeap expirations_;
    std::size_t capacity_;
};