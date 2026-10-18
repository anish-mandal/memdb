#include "hash_table.hpp"

HashTable::HashTable(std::size_t bucket_count)
    : buckets_(bucket_count == 0 ? 1 : bucket_count), entry_count_(0) {}

void HashTable::insert_or_assign(const std::string& key,
                                 const std::string& value) {
    std::list<CacheEntry>& bucket = buckets_[bucket_index(key)];

    for (CacheEntry& entry : bucket) {
        if (entry.key == key) {
            entry.value = value;
            return;
        }
    }


    bucket.push_back(CacheEntry{
        key,
        value,
        std::chrono::steady_clock::now(),
        std::chrono::steady_clock::time_point{},
        false
    });
    ++entry_count_;
}

CacheEntry* HashTable::find(const std::string& key) {
    std::list<CacheEntry>& bucket = buckets_[bucket_index(key)];

    for (CacheEntry& entry : bucket) {
        if (entry.key == key) {
            return &entry;
        }
    }
    return nullptr;
}

const CacheEntry* HashTable::find(const std::string& key) const {
    const std::list<CacheEntry>& bucket = buckets_[bucket_index(key)];

    for (const CacheEntry& entry : bucket) {
        if (entry.key == key) {
            return &entry;
        }
    }
    return nullptr;
}

bool HashTable::contains(const std::string& key) const {
    return find(key) != nullptr;
}

bool HashTable::remove(const std::string& key) {
    std::list<CacheEntry>& bucket = buckets_[bucket_index(key)];

    for (auto iterator = bucket.begin(); iterator != bucket.end(); ++iterator) {
        if (iterator->key == key) {
            bucket.erase(iterator);
            --entry_count_;
            return true;
        }
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
    for (std::list<CacheEntry>& bucket : buckets_) {
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
