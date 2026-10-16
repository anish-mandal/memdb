#include "min_heap.hpp"
#include <algorithm>
#include <stdexcept>

void MinHeap::push(const ExpirationRecord& record) {
    heap_.push_back(record);
    std::push_heap(heap_.begin(), heap_.end(), std::greater<ExpirationRecord>{});
}
bool MinHeap::empty() const { return heap_.empty(); }
std::size_t MinHeap::size() const { return heap_.size(); }
const ExpirationRecord& MinHeap::top() const {
    if (heap_.empty()) throw std::out_of_range("empty expiration heap");
    return heap_.front();
}
ExpirationRecord MinHeap::pop() {
    if (heap_.empty()) throw std::out_of_range("empty expiration heap");
    std::pop_heap(heap_.begin(), heap_.end(), std::greater<ExpirationRecord>{});
    ExpirationRecord result = heap_.back();
    heap_.pop_back();
    return result;
}
void MinHeap::clear() { heap_.clear(); }
