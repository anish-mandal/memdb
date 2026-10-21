#include "hash_table.hpp"

HashTable::HashTable(std::size_t bucket_count)
    : buckets_(bucket_count == 0 ? 1 : bucket_count), entry_count_(0) {}

void HashTable::insert_or_assign(const std::string& key,
                                 const std::string& value) {
    LinkedList& bucket = buckets_[bucket_index(key)];
    const bool existing = bucket.contains(key);
    bucket.insert_or_assign(key, value);
    if (!existing) {
        ++entry_count_;
    }
}

CacheEntry* HashTable::find(const std::string& key) {
    return buckets_[bucket_index(key)].find(key);
}

const CacheEntry* HashTable::find(const std::string& key) const {
    return buckets_[bucket_index(key)].find(key);
}

bool HashTable::contains(const std::string& key) const {
    return find(key) != nullptr;
}

bool HashTable::remove(const std::string& key) {
    if (buckets_[bucket_index(key)].remove(key)) {
        --entry_count_;
        return true;
    }
    return false;
}

std::size_t HashTable::size() const {
    return entry_count_;
}

bool HashTable::empty() const {
    return entry_count_ == 0;
}

void HashTable::clear() {
    for (LinkedList& bucket : buckets_) {
        bucket.clear();
    }
    entry_count_ = 0;
}

std::size_t HashTable::bucket_index(const std::string& key) const {
    std::size_t hash = 14695981039346656037ull;

    for (unsigned char character : key) {
        hash ^= character;
        hash *= 1099511628211ull;
    }

    return hash % buckets_.size();
}
