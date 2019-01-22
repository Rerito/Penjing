

add_executable(
    unittest
    test_lru_cache.cpp
    test_disjoint_sets.cpp
    test_concepts.cpp
    test_memory.cpp
    test_suffix_tree.cpp
    main.cpp
)

target_link_libraries(
    unittest
    suffix_tree
    gtest_main
)


add_test(NAME AllUnitTests COMMAND unittest)
