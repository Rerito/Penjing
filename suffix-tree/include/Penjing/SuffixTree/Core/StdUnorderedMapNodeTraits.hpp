// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <unordered_map>

#include <Penjing/Meta/CurryTemplate.hpp>

namespace Penjing {
namespace SuffixTree {
namespace Core {

// NodeTraits implementation using std::unordered_map.
// The template takes 3 parameters:
//  - StorageAlloc The allocator for the node's internal map (rebound using
//    std::allocator_traits) of transitions.
//  - Hash A Kvasir style metafunction returning the hash type for the map.
//    default: A metafunction returning `std::hash< T >` for the input type T
//  - KeyEqual A Kvasir style metafunction returning the comparison type for
//    the map.
//    default: A metafunction returning `std::equal_to< T >`
//    for the input type T
template<
    typename StorageAlloc,
    typename Hash = Meta::CurryTemplate< std::hash >,
    typename KeyEqual = Meta::CurryTemplate< std::equal_to > >
struct StdUnorderedMapNodeTraits
{
    using TransitionAllocator = StorageAlloc;

    template< typename K, typename V >
    using StorageType = std::unordered_map<
        K,
        V,
        typename Hash::template f< K >,
        typename KeyEqual::template f< K >,
        typename std::allocator_traits< StorageAlloc >::template rebind_alloc<
            std::pair< K const, V > > >;
};

} // namespace Core
} // namespace SuffixTree
} // namespace Penjing
