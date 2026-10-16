#pragma once

#include <optional>
#include <string>

class EvictionPolicy {
public:
    virtual ~EvictionPolicy() = default;
    virtual void on_insert(const std::string& key) = 0;
    virtual void on_access(const std::string& key) = 0;
    virtual void on_remove(const std::string& key) = 0;
    virtual std::optional<std::string> evict_key() const = 0;
    virtual void clear() {}
};