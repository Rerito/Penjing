// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#include <type_traits>

#include "IsDetected.hpp"

namespace Penjing::Meta {

namespace Details {

template< typename U, typename V >
using NotEqualType = decltype(std::declval< U >() != std::declval< V >());

template< typename U, typename V >
using EqualType = decltype(std::declval< U >() = std::declval< V >());

} // namespace Details

template< typename U, typename V >
using IsNotEqualComparable = std::bool_constant<
    IsDetected< Details::NotEqualType, U, V >::value &&
    IsDetected< Details::NotEqualType, V, U >::value >;

template< typename U, typename V >
using IsEqualComparable = std::bool_constant<
    IsDetected< Details::EqualType, U, V >::value &&
    IsDetected< Details::EqualType, V, U >::value >;

template< typename U, typename V >
using IsComparable = std::bool_constant<
    IsEqualComparable< U, V >::value && IsNotEqualComparable< U, V >::value >;

} // namespace Penjing::Meta
