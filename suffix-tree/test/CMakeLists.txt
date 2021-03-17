# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_executable(
    penjing-suffix-tree-test
    src/Algorithm/CompareNodes.cpp
    src/Algorithm/Walk.cpp
    src/BananaFixture.cpp
    src/main.cpp
    src/Concepts.cpp
    src/NodeStore.cpp
    src/NodeTraits.cpp
    src/SuffixTree.cpp
    src/Ukkonen/Build.cpp
    src/Ukkonen/Canonize.cpp
    src/Ukkonen/TestAndSplit.cpp
    src/Ukkonen/Split.cpp
    src/Ukkonen/Update.cpp)

target_include_directories(
    penjing-suffix-tree-test
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(
    penjing-suffix-tree-test
    penjing-suffix-tree
    gtest_main)

add_test(NAME PenjingSuffixTreeTests COMMAND penjing-suffix-tree-test)
