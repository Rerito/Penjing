# Copyright (c) 2021-2022, Rerito
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
        penjing::meta
        penjing::storage)

add_library(penjing::suffixtree ALIAS penjing-suffixtree)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(test)
