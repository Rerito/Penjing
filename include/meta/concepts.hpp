
#pragma once

#include <type_traits>
#include <utility> // for integer_sequence
#include "void_t.hpp"

namespace meta {

template <bool... Bs>
using all_of_t = std::is_same<std::integer_sequence<bool, true, Bs...>, std::integer_sequence<bool, Bs..., true> >;

namespace detail {

template <typename, typename = void>
struct models_single : std::false_type {};

template <typename Concept, typename T, typename... Args>
struct models_single<
    Concept(T, Args...),
    void_t<
        decltype(std::declval<Concept>().requires_(
              std::declval<T>(),
              std::declval<Args>()...
              )
        )
    > > : std::true_type {};

} // namespace detail

template <typename... Concepts>
struct models : all_of_t<detail::models_single<Concepts>::value...> {};


template <typename... Concepts>
struct models_base {
    static_assert(models<Concepts...>::value, "One of the given concepts is not enforced");
};
} // namespace meta
