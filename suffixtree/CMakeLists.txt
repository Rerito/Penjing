# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_library(penjing-suffixtree INTERFACE)

target_include_directories(
    penjing-suffixtree
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(
    penjing-suffixtree
    INTERFACE
        penjing-meta )

add_subdirectory(test)
