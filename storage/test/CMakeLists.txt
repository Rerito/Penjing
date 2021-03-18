# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_executable(
    penjing-storage-test
    src/Store.cpp)

target_link_libraries(
    penjing-storage-test
    penjing-storage
    gtest_main)

add_test(NAME PenjingStorageTests COMMAND penjing-storage-test)
