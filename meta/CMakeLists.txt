# Copyright (c) 2021-2022, Rerito
# SPDX-License-Identifier: MIT

add_library(penjing-meta INTERFACE)

target_include_directories(
    penjing-meta
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

add_library(penjing::meta ALIAS penjing-meta)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(test)
