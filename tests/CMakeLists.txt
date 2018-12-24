
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(
    test
    test_lru_cache.cpp
    main.cpp
)

target_link_libraries(
    test
    gtest_main
)
