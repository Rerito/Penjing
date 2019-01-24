
#pragma once

#include <type_traits>
#include "concept_union.hpp"
#include "has_size.hpp"
#include "has_data.hpp"

namespace meta {

namespace detail {

template <typename StrType, typename SizeType>
struct is_constructible {
    template <typename T>
    auto requires_(T&&) -> std::enable_if_t<std::is_constructible<T, StrType, SizeType>::value>;
};

} // namespace detail

template <typename StrType, typename SizeType>
using is_string_view = concept_union<detail::is_constructible<StrType, SizeType>, adl_concepts::has_size, adl_concepts::has_data>;


} // namespace meta
