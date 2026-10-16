#include "cache_manager.hpp"
#include <cassert>
#include <chrono>
#include <thread>

int main() {
    CacheManager cache(4);
    cache.set("short", "value", std::chrono::milliseconds(20));
    assert(cache.contains("short"));
    std::this_thread::sleep_for(std::chrono::milliseconds(35));
    assert(!cache.contains("short"));
    assert(!cache.set("instant", "x", std::chrono::milliseconds(0)));
    assert(!cache.contains("instant"));
    return 0;
}
