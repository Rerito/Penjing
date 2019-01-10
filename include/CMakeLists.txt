
add_library(suffix_tree INTERFACE)

#target_sources(suffix_tree INTERFACE
#    cache.hpp
#    concepts.hpp
#    disjoint_sets.hpp
#    forward_macro.hpp
#    has_size.hpp
#    lru_cache.hpp
#    void_t.hpp
#)

set(SUFFIX_TREE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/include")
message(STATUS "${SUFFIX_TREE_SOURCE_DIR}")
target_include_directories(suffix_tree INTERFACE "${SUFFIX_TREE_SOURCE_DIR}")

# target_link_libraries(suffix_tree hana)
