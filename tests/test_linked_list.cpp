#include "linked_list.hpp"

#include <cassert>

int main() {
    LinkedList list;

    assert(list.empty());
    assert(list.size() == 0);
    assert(list.find("missing") == nullptr);
    assert(!list.contains("missing"));

    list.insert_or_assign("name", "MemDB");
    assert(!list.empty());
    assert(list.size() == 1);
    assert(list.contains("name"));
    assert(list.find("name") != nullptr);
    assert(list.find("name")->value == "MemDB");

    list.insert_or_assign("name", "Updated MemDB");
    assert(list.size() == 1);
    assert(list.find("name")->value == "Updated MemDB");

    list.insert_or_assign("language", "C++");
    assert(list.size() == 2);
    assert(list.find("language")->value == "C++");

    const LinkedList& read_only_list = list;
    const CacheEntry* entry = read_only_list.find("name");
    assert(entry != nullptr);
    assert(entry->value == "Updated MemDB");
    assert(read_only_list.contains("language"));

    assert(!list.remove("missing"));
    assert(list.size() == 2);
    assert(list.remove("name"));
    assert(!list.contains("name"));
    assert(list.size() == 1);
    assert(!list.remove("name"));

    list.clear();
    assert(list.empty());
    assert(list.size() == 0);
    assert(list.find("language") == nullptr);

    return 0;
}
