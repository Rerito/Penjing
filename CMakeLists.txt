
cmake_minimum_required(VERSION 3.9)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(suffix-tree
    VERSION 1.0
    DESCRIPTION "A C++ Suffix Tree Implementation"
    LANGUAGES CXX
)

enable_testing()

# Only require C++14 for thirdparties
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/include)
add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparties)

# ... But we do want C++17 for our own stuff
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
