

#pragma once

#include <utility>

#include "type_list.hpp"
#include "utils/forward_macro.hpp"

namespace meta {

template <typename... Concepts>
struct concept_union {
    template <typename... Args>
    auto requires_(Args&&... args) -> type_list<decltype(std::declval<Concepts>().requires_(CPPFWD(args)...))...>;
};

} // namespace meta
