# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_executable(
    penjing-meta-test
    src/main.cpp
    src/Mapped.cpp)

target_link_libraries(
    penjing-meta-test
    penjing-meta
    gtest_main)

add_test(NAME PenjingMetaTests COMMAND penjing-meta-test)
