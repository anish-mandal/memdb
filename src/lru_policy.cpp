 #include "lru_policy.hpp"  

void LRUPolicy::on_insert(const std::string& key) {
    on_remove(key);
    order_.push_front(key);
    positions_[key] = order_.begin();
}
void LRUPolicy::on_access(const std::string& key) {
    auto it = positions_.find(key);
    if (it == positions_.end()) return;
    order_.splice(order_.begin(), order_, it->second);
    it->second = order_.begin();
}
void LRUPolicy::on_remove(const std::string& key) {
    auto it = positions_.find(key);
    if (it != positions_.end()) {
        order_.erase(it->second);
        positions_.erase(it);
    }
}
std::optional<std::string> LRUPolicy::evict_key() const {
    if (order_.empty()) return std::nullopt;
    return order_.back();
}
std::size_t LRUPolicy::size() const { return order_.size(); }
void LRUPolicy::clear() { order_.clear(); positions_.clear(); }
