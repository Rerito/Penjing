# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

add_library(meta INTERFACE)
target_include_directories(
    meta
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)
