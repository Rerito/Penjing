# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(
    Penjing
    VERSION 1.0
    DESCRIPTION "A C++ Header-Only library exposing suffix tree algorithms"
    LANGUAGES CXX
)

enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(thirdparties)
add_subdirectory(meta)
add_subdirectory(storage)
add_subdirectory(suffixtree)
