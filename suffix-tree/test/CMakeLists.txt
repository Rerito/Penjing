# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_executable(
    suffix-tree-test
    src/BananaFixture.cpp
    src/main.cpp
    src/Concepts.cpp
    src/NodeStore.cpp
    src/NodeTraits.cpp
    src/SuffixTree.cpp
    src/Ukkonen/Canonize.cpp
    src/Walk.cpp)

target_include_directories(
    suffix-tree-test
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(
    suffix-tree-test
    suffix-tree
    gtest_main)
