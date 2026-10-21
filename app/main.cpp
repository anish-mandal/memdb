#include "cache_manager.hpp"

#include <charconv>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

void print_help() {
    std::cout
        << "Commands:\n"
        << "  SET <key> <value> [ttl_ms]  Store or update a value\n"
        << "  GET <key>                   Retrieve a value\n"
        << "  DELETE <key>                Remove a key\n"
        << "  EXISTS <key>                Check whether a key exists\n"
        << "  SIZE                        Show the number of entries\n"
        << "  PURGE                       Remove expired entries\n"
        << "  CLEAR                       Remove all entries\n"
        << "  HELP                        Show this help\n"
        << "  EXIT                        Exit MemDB\n"
        << "\nValues containing spaces must be enclosed in double quotes.\n";
}

template <typename Integer>
bool parse_integer(const std::string& text, Integer& result) {
    const char* first = text.data();
    const char* last = first + text.size();
    const auto parsed = std::from_chars(first, last, result);
    return parsed.ec == std::errc{} && parsed.ptr == last;
}

bool read_value(std::istringstream& input, std::string& value) {
    input >> std::ws;
    if (input.peek() == '"') {
        input >> std::quoted(value);
        return !input.fail();
    }
    return static_cast<bool>(input >> value);
}

}  // namespace

int main(int argc, char* argv[]) {
    std::size_t capacity = 100;
    EvictionPolicyType policy = EvictionPolicyType::LRU;

    if (argc > 3) {
        std::cerr << "Usage: memdb_app [capacity] [lru|fifo]\n";
        return 1;
    }

    if (argc >= 2 && !parse_integer(argv[1], capacity)) {
        std::cerr << "Invalid capacity: " << argv[1] << '\n';
        return 1;
    }

    if (argc == 3) {
        const std::string policy_name = argv[2];
        if (policy_name == "lru") {
            policy = EvictionPolicyType::LRU;
        } else if (policy_name == "fifo") {
            policy = EvictionPolicyType::FIFO;
        } else {
            std::cerr << "Invalid policy. Use lru or fifo.\n";
            return 1;
        }
    }

    CacheManager cache(capacity, policy);
    std::cout << "MemDB CLI (capacity: " << capacity
              << ", policy: "
              << (policy == EvictionPolicyType::LRU ? "LRU" : "FIFO") << ")\n";
    std::cout << "Type HELP for available commands.\n";

    std::string line;
    while (true) {
        std::cout << "memdb> " << std::flush;
        if (!std::getline(std::cin, line)) {
            std::cout << '\n';
            break;
        }

        std::istringstream input(line);
        std::string command;
        input >> command;
        if (command.empty()) {
            continue;
        }

        if (command == "SET") {
            std::string key;
            std::string value;
            std::string ttl_text;
            if (!(input >> key) || !read_value(input, value)) {
                std::cout << "Usage: SET <key> <value> [ttl_ms]\n";
                continue;
            }

            if (input >> ttl_text) {
                long long ttl = 0;
                if (!parse_integer(ttl_text, ttl) || ttl < 0) {
                    std::cout
                        << "TTL must be a non-negative integer in milliseconds.\n";
                    continue;
                }
                if (!cache.set(key, value, std::chrono::milliseconds(ttl))) {
                    std::cout << "SET failed.\n";
                    continue;
                }
            } else if (!cache.set(key, value)) {
                std::cout << "SET failed.\n";
                continue;
            }
            std::cout << "OK\n";
        } else if (command == "GET") {
            std::string key;
            if (!(input >> key)) {
                std::cout << "Usage: GET <key>\n";
                continue;
            }
            const auto value = cache.get(key);
            std::cout << (value ? *value : "(nil)") << '\n';
        } else if (command == "DELETE") {
            std::string key;
            if (!(input >> key)) {
                std::cout << "Usage: DELETE <key>\n";
                continue;
            }
            std::cout << (cache.remove(key) ? "1" : "0") << '\n';
        } else if (command == "EXISTS") {
            std::string key;
            if (!(input >> key)) {
                std::cout << "Usage: EXISTS <key>\n";
                continue;
            }
            std::cout << (cache.contains(key) ? "1" : "0") << '\n';
        } else if (command == "SIZE") {
            std::cout << cache.size() << '\n';
        } else if (command == "PURGE") {
            std::cout << cache.purge_expired() << '\n';
        } else if (command == "CLEAR") {
            cache.clear();
            std::cout << "OK\n";
        } else if (command == "HELP") {
            print_help();
        } else if (command == "EXIT" || command == "QUIT") {
            break;
        } else {
            std::cout << "Unknown command. Type HELP for available commands.\n";
        }
    }

    return 0;
}
