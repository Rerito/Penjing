// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <type_traits>
#include <variant>

#include "IsDetected.hpp"

namespace Penjing {
namespace Meta {

namespace Details {

template< typename T >
inline constexpr char const* toCharPtr(T const* ptr)
{
    return static_cast< char const* >(static_cast< void const* >(ptr));
}

template< typename... Args >
std::variant< Args... > isConvertibleToVariant(std::variant< Args... >&);

template< typename T >
using ToVariant = decltype(isConvertibleToVariant(
    std::declval< std::add_lvalue_reference_t< T > >()));

} // namespace Details

template< typename T >
concept ConvertibleToVariant = Detected< Details::ToVariant, T >;

// Constexpr function to evaluate the offset of a `std::variant`'s storage.
// This is a bit hackish as it assumes the variant's storage will always have
// the same offset regardless of the variant's types.
// This is usually the case with STL implementations.
// For this to work, all the types in the variant need to be constexpr
// constructible.
template< typename Variant >
    requires ConvertibleToVariant< Variant >
inline constexpr size_t variantStorageOffset()
{
    constexpr Variant v{};

    return Details::toCharPtr(std::get_if< 0 >(std::addressof(v))) -
           Details::toCharPtr(std::addressof(v));
}

template< typename Variant >
inline constexpr size_t VARIANT_STORAGE_OFFSET =
    variantStorageOffset< Variant >();

template< typename Variant >
concept NoStorageOffsetVariant = (VARIANT_STORAGE_OFFSET< Variant > == 0);

} // namespace Meta
} // namespace Penjing
