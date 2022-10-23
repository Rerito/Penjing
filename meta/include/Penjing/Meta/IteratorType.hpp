// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#include <iterator>
#include <type_traits>

#include "DetectedOr.hpp"

#pragma once

namespace Penjing::Meta {

namespace Details {

template< typename T >
constexpr auto iteratorType(T&& obj)
{
    using std::begin;
    return begin(obj);
}

} // namespace Details

template< typename T >
using IteratorType = decltype(Details::iteratorType(std::declval< T >()));

} // namespace Penjing::Meta
