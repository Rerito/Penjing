// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <type_traits>

namespace Penjing {
namespace Meta {

namespace Details {

template< template< typename... > class, typename, typename... >
struct IsDetected : std::false_type
{
};

template< template< typename... > class Z, typename... Args >
struct IsDetected< Z, std::void_t< Z< Args... > >, Args... > : std::true_type
{
};

} // namespace Details

// Detection idiom for the given template Z and arguments Args...
// Member value evaluates to true if `Z< Args... >` is a valid specialization
// of `Z`.
template< template< typename... > class Z, typename... Args >
using IsDetected = Details::IsDetected< Z, void, Args... >;

template< template< typename... > class Z, typename... Args >
constexpr bool Detected = IsDetected< Z, Args... >::value;

} // namespace Meta
} // namespace Penjing
