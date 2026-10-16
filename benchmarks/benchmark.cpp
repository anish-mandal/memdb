#include "cache_manager.hpp"
#include <chrono>
#include <iostream>

int main() {
    CacheManager cache(10000);
    constexpr int operations = 100000;
    const auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < operations; ++i) {
        cache.set("key" + std::to_string(i % 10000), "value");
        cache.get("key" + std::to_string(i % 10000));
    }
    const auto elapsed = std::chrono::duration<double, std::micro>(
        std::chrono::steady_clock::now() - start).count();
    std::cout << operations * 2 << " operations in " << elapsed << " us\n";
}
