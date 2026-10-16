#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

struct ExpirationRecord {
    std::chrono::steady_clock::time_point expires_at;
    std::string key;
    bool operator>(const ExpirationRecord& other) const {
        return expires_at > other.expires_at;
    }
};

class MinHeap {
public:
    void push(const ExpirationRecord& record);
    bool empty() const;
    std::size_t size() const;
    const ExpirationRecord& top() const;
    ExpirationRecord pop();
    void clear();
private:
    std::vector<ExpirationRecord> heap_;
};