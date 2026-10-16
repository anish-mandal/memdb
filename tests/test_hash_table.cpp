#include "hash_table.hpp"

#include <cassert>
#include <string>

int main() {
    HashTable table(1);

    assert(table.empty());
    assert(table.size() == 0);
    assert(!table.contains("missing"));
    assert(table.find("missing") == nullptr);

    table.insert_or_assign("name", "MemDB");
    assert(!table.empty());
    assert(table.size() == 1);
    assert(table.contains("name"));
    assert(table.find("name") != nullptr);
    assert(table.find("name")->value == "MemDB");

    table.insert_or_assign("name", "Updated MemDB");
    assert(table.size() == 1);
    assert(table.find("name")->value == "Updated MemDB");

    table.insert_or_assign("language", "C++");
    assert(table.size() == 2);
    assert(table.find("language")->value == "C++");

    const HashTable& read_only_table = table;
    const CacheEntry* entry = read_only_table.find("name");
    assert(entry != nullptr);
    assert(entry->value == "Updated MemDB");
    assert(read_only_table.contains("language"));

    assert(!table.remove("missing"));
    assert(table.size() == 2);
    assert(table.remove("name"));
    assert(!table.contains("name"));
    assert(table.size() == 1);
    assert(!table.remove("name"));

    table.clear();
    assert(table.empty());
    assert(table.size() == 0);
    assert(table.find("language") == nullptr);

    return 0;
}