// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#include <iterator>
#include <type_traits>

#include "DetectedOr.hpp"
#include "IsComparable.hpp"
#include "IsIterator.hpp"

#pragma once

namespace Penjing::Meta {

template< typename BeginIt, typename EndIt >
class IteratorRange
{
    static_assert(IsIterator< BeginIt >::value && IsIterator< EndIt >::value);
    static_assert(IsComparable< BeginIt, EndIt >::value);

    BeginIt _b;
    EndIt _e;

public:
    constexpr IteratorRange(BeginIt b, EndIt e)
        : _b(b)
        , _e(e)
    {
    }

    constexpr auto begin() const { return _b; }

    constexpr auto end() const { return _e; }
};

template< typename BeginIt, typename EndIt >
constexpr auto makeIteratorRange(std::pair< BeginIt, EndIt > iterators)
{
    return IteratorRange< BeginIt, EndIt >(iterators.first, iterators.second);
}

} // namespace Penjing::Meta
