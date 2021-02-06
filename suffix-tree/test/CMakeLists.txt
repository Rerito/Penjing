# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_executable(
    suffix-tree-test
    src/concepts.cpp
    src/main.cpp)

target_link_libraries(
    suffix-tree-test
    suffix-tree
    gtest_main)
