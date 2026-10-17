 
#include "fifo_policy.hpp"

void FIFOPolicy::on_insert(const std::string& key) {
    on_remove(key);
    order_.push_back(key);
    present_.insert(key);
}
void FIFOPolicy::on_remove(const std::string& key) {
    if (present_.erase(key) == 0) return;
    for (auto it = order_.begin(); it != order_.end(); ++it) {
        if (*it == key) { order_.erase(it); break; }
    }
}
std::optional<std::string> FIFOPolicy::evict_key() const {
    if (order_.empty()) return std::nullopt;
    return order_.front();
}
std::size_t FIFOPolicy::size() const { return present_.size(); }
void FIFOPolicy::clear() { order_.clear(); present_.clear(); }
