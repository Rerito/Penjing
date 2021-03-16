# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_executable(
    meta-test
    src/main.cpp
    src/Mapped.cpp)

target_link_libraries(
    meta-test
    meta
    gtest_main)

add_test(NAME MetaTests COMMAND meta-test)
