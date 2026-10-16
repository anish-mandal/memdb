cmake_minimum_required(VERSION 3.16)

project(MemDB
    VERSION 1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(memdb
    src/cache_manager.cpp
    src/fifo_policy.cpp
    src/hash_table.cpp
    src/linked_list.cpp
    src/lru_policy.cpp
    src/min_heap.cpp
)

target_include_directories(memdb
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

enable_testing()

add_executable(test_hash_table
    tests/test_hash_table.cpp
)

target_link_libraries(test_hash_table
    PRIVATE
        memdb
)

add_test(
    NAME hash_table_tests
    COMMAND test_hash_table
)

add_executable(test_linked_list
    tests/test_linked_list.cpp
)

target_link_libraries(test_linked_list
    PRIVATE
        memdb
)

add_test(
    NAME linked_list_tests
    COMMAND test_linked_list
)

add_executable(test_cache tests/test_cache.cpp)
target_link_libraries(test_cache PRIVATE memdb)
add_test(NAME cache_tests COMMAND test_cache)

add_executable(test_eviction tests/test_eviction.cpp)
target_link_libraries(test_eviction PRIVATE memdb)
add_test(NAME eviction_tests COMMAND test_eviction)

add_executable(test_ttl tests/test_ttl.cpp)
target_link_libraries(test_ttl PRIVATE memdb)
add_test(NAME ttl_tests COMMAND test_ttl)

add_executable(memdb_app app/main.cpp)
target_link_libraries(memdb_app PRIVATE memdb)

add_executable(memdb_benchmark benchmarks/benchmark.cpp)
target_link_libraries(memdb_benchmark PRIVATE memdb)