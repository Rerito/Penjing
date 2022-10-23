// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#include <iterator>
#include <type_traits>

#include "DetectedOr.hpp"

#pragma once

namespace Penjing::Meta {

namespace Details {

template< typename T >
using ValueType = typename T::value_type;

template< typename T >
constexpr decltype(auto) valueType(T&& obj)
{
    using std::begin;
    return *begin(obj);
}

template< typename T >
using ValueTypeFromBegin =
    std::remove_reference_t< decltype(valueType(std::declval< T >())) >;

} // namespace Details

template< typename T >
using ValueType =
    DetectedOr< Details::ValueTypeFromBegin< T >, Details::ValueType, T >;

} // namespace Penjing::Meta
