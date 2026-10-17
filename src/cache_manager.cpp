#include "cache_manager.hpp"
#include "fifo_policy.hpp"
#include "lru_policy.hpp"

CacheManager::CacheManager(std::size_t capacity, EvictionPolicyType type)
    : CacheManager(capacity, type == EvictionPolicyType::LRU
        ? std::unique_ptr<EvictionPolicy>(new LRUPolicy)
        : std::unique_ptr<EvictionPolicy>(new FIFOPolicy)) {}

CacheManager::CacheManager(std::size_t capacity,
                           std::unique_ptr<EvictionPolicy> policy)
    : policy_(std::move(policy)), capacity_(capacity) {
    if (!policy_) policy_ = std::unique_ptr<EvictionPolicy>(new LRUPolicy);
}

bool CacheManager::set(const std::string& key, const std::string& value) {
    expire_entries();
    if (capacity_ == 0) return false;
    CacheEntry* entry = table_.find(key);
    const bool existing = entry != nullptr;
    table_.insert_or_assign(key, value);
    entry = table_.find(key);
    entry->created_at = std::chrono::steady_clock::now();
    entry->has_expiration = false;
    entry->expires_at = {};
    if (existing) policy_->on_access(key); else policy_->on_insert(key);
    evict_if_needed();
    return table_.contains(key);
}

bool CacheManager::set(const std::string& key, const std::string& value,
                       std::chrono::milliseconds ttl) {
    if (!set(key, value)) return false;
    CacheEntry* entry = table_.find(key);
    entry->has_expiration = true;
    entry->expires_at = entry->created_at + ttl;
    expirations_.push({entry->expires_at, key});
    expire_entries();
    return table_.contains(key);
}

void CacheManager::expire_entries() {
    const auto now = std::chrono::steady_clock::now();
    while (!expirations_.empty() && expirations_.top().expires_at <= now) {
        const ExpirationRecord record = expirations_.pop();
        CacheEntry* entry = table_.find(record.key);
        if (entry && entry->has_expiration && entry->expires_at == record.expires_at) {
            policy_->on_remove(record.key);
            table_.remove(record.key);
        }
    }
}

std::size_t CacheManager::purge_expired() {
    const auto before = table_.size();
    expire_entries();
    return before - table_.size();
}

void CacheManager::evict_if_needed() {
    while (table_.size() > capacity_) {
        const std::optional<std::string> key = policy_->evict_key();
        if (!key) break;
        policy_->on_remove(*key);
        table_.remove(*key);
    }
}

std::optional<std::string> CacheManager::get(const std::string& key) {
    expire_entries();
    CacheEntry* entry = table_.find(key);
    if (!entry) return std::nullopt;
    policy_->on_access(key);
    return entry->value;
}
bool CacheManager::get(const std::string& key, std::string& value) {
    auto result = get(key);
    if (!result) return false;
    value = *result;
    return true;
}
bool CacheManager::contains(const std::string& key) {
    expire_entries();
    return table_.contains(key);
}
bool CacheManager::remove(const std::string& key) {
    expire_entries();
    if (!table_.contains(key)) return false;
    policy_->on_remove(key);
    return table_.remove(key);
}
void CacheManager::clear() {
    table_.clear();
    policy_->clear();
    expirations_.clear();
}
std::size_t CacheManager::size() {
    expire_entries();
    return table_.size();
}
