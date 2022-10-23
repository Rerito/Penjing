// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include "IsDetected.hpp"

namespace Penjing::Meta {

namespace Details {

template< bool, typename Default, template< typename... > class, typename... >
struct DetectedOrImpl
{
    using Type = Default;
};

template< typename Default, template< typename... > class Z, typename... Args >
struct DetectedOrImpl< true, Default, Z, Args... >
{
    using Type = Z< Args... >;
};

} // namespace Details

template< typename Default, template< typename... > class Z, typename... Args >
using DetectedOr = typename Details::
    DetectedOrImpl< Detected< Z, Args... >, Default, Z, Args... >::Type;

} // namespace Penjing::Meta
