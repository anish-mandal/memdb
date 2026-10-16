#pragma once

#include "eviction_policy.hpp"
#include <list>
#include <string>
#include <unordered_map>

class LRUPolicy final : public EvictionPolicy {
public:
    void on_insert(const std::string& key) override;
    void on_access(const std::string& key) override;
    void on_remove(const std::string& key) override;
    std::optional<std::string> evict_key() const override;
    void clear() override;
    std::size_t size() const;
private:
    std::list<std::string> order_;
    std::unordered_map<std::string, std::list<std::string>::iterator> positions_;
};