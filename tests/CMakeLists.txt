

add_executable(
    unittest
    test_lru_cache.cpp
    test_disjoint_sets.cpp
    test_concepts
    main.cpp
)

target_link_libraries(
    unittest
    suffix_tree
    gtest_main
)


add_test(NAME AllUnitTests COMMAND unittest)
