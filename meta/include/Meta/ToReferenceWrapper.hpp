// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <type_traits>

namespace Meta {

// Convert a reference type to the corresponding reference_wrapper type.
template< typename T >
    requires std::is_reference_v< T >
using ToReferenceWrapper =
    std::reference_wrapper< std::remove_reference_t< T > >;

} // namespace Meta
