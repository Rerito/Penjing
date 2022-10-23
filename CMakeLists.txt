# Copyright (c) 2021-2022, Rerito
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(
    Penjing
    VERSION 1.0
    DESCRIPTION "A C++ Header-Only library exposing suffix tree algorithms"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_CXX_STANDARD)
    message(STATUS "No CMAKE_CXX_STANDARD defined, defaulting to 20")
    set(CMAKE_CXX_STANDARD 17)
endif()

find_package(GTest 1.11 REQUIRED)

enable_testing()

add_subdirectory(meta)
add_subdirectory(storage)
add_subdirectory(suffixtree)
