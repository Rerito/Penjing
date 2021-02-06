# Copyright (c) 2021, Rerito
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(suffix-tree
    VERSION 1.0
    DESCRIPTION "A C++ Suffix Tree Implementation"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparties)
add_subdirectory(${CMAKE_SOURCE_DIR}/suffix-tree)
