#include "cache_manager.hpp"
#include <cassert>
#include <chrono>
#include <thread>

int main() {
    CacheManager cache(2);
    assert(cache.set("a", "1"));
    assert(cache.set("b", "2"));
    assert(cache.get("a") == "1");
    assert(cache.set("c", "3"));
    assert(!cache.contains("b"));
    assert(cache.contains("a") && cache.contains("c"));
    cache.remove("a");
    assert(cache.size() == 1);
    cache.clear();
    assert(cache.empty());
    return 0;
}
