# Copyright (c) 2021-2022, Rerito
# SPDX-License-Identifier: MIT

add_library(penjing-storage INTERFACE)

target_include_directories(
    penjing-storage
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

target_link_libraries(
    penjing-storage
    INTERFACE
        penjing::meta)

add_library(penjing::storage ALIAS penjing-storage)

install(
    DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

