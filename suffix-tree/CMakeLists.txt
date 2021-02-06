# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_library(suffix-tree INTERFACE)
target_include_directories(
    suffix-tree
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

add_subdirectory(test)
