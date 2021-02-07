# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_library(suffix-tree INTERFACE)
target_include_directories(
    suffix-tree
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(
    suffix-tree
    INTERFACE
        meta )

add_subdirectory(test)
