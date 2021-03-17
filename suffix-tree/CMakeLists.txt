# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_library(penjing-suffix-tree INTERFACE)
target_include_directories(
    penjing-suffix-tree
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(
    penjing-suffix-tree
    INTERFACE
        penjing-meta )

add_subdirectory(test)
