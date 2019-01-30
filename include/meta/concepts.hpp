
#pragma once

#include <type_traits>
#include <utility> // for integer_sequence

#include "type_list.hpp"
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

namespace detail {

template <typename Concept>
struct models_base_single {
    static_assert(models_single<Concept>::value, "The given concept is not enforced");
};

} // namespace detail

template <typename... Concepts>
struct models_base : detail::models_base_single<Concepts>... {};

} // namespace meta
