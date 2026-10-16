#pragma once

#include <chrono>
#include <string>

struct CacheEntry {
    std::string key, value;
    std::chrono::steady_clock::time_point created_at, expires_at;
    bool has_expiration;
};
