#include "cache_manager.hpp"
#include <cassert>

int main() {
    CacheManager fifo(2, EvictionPolicyType::FIFO);
    fifo.set("a", "1"); fifo.set("b", "2");
    fifo.get("a");
    fifo.set("c", "3");
    assert(!fifo.contains("a"));
    assert(fifo.contains("b") && fifo.contains("c"));
    return 0;
}
