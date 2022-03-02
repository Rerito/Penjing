# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_executable(
    penjing-suffixtree-test
    src/Algorithm/CompareNodes.cpp
    src/Algorithm/Walk.cpp
    src/BananaFixture.cpp
    src/main.cpp
    src/Concepts.cpp
    src/SuffixTree.cpp
    src/Ukkonen/Build.cpp
    src/Ukkonen/Canonize.cpp
    src/Ukkonen/TestAndSplit.cpp
    src/Ukkonen/Split.cpp
    src/Ukkonen/Update.cpp)

target_include_directories(
    penjing-suffixtree-test
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_link_libraries(
    penjing-suffixtree-test
    penjing-suffixtree
    GTest::GTest)

add_test(NAME PenjingSuffixTreeTests COMMAND penjing-suffixtree-test)
