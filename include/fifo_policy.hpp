#pragma once

#include "eviction_policy.hpp"
#include <deque>
#include <string>
#include <unordered_set>

class FIFOPolicy final : public EvictionPolicy {
public:
    void on_insert(const std::string& key) override;
    void on_access(const std::string&) override {}
    void on_remove(const std::string& key) override;
    std::optional<std::string> evict_key() const override;
    void clear() override;
    std::size_t size() const;
private:
    std::deque<std::string> order_;
    std::unordered_set<std::string> present_;
};

