
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
    unittest
    test_lru_cache.cpp
    main.cpp
)

target_link_libraries(
    unittest
    gtest_main
)

add_test(NAME AllUnitTests COMMAND unittest)
