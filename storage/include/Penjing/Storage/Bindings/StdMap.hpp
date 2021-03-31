// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <map>
#include <memory>

#include <Penjing/Meta/CurryTemplate.hpp>

namespace Penjing {
namespace Storage {
namespace Bindings {

template<
    typename StorageAlloc,
    typename Compare = Meta::CurryTemplate< std::less > >
class StdMap
{
public:
    using AllocatorType = StorageAlloc;

    template< typename K, typename V >
    using StorageType = std::map<
        K,
        V,
        typename Compare::template f< K >,
        typename std::allocator_traits< StorageAlloc >::template rebind_alloc<
            std::pair< K const, V > > >;
};

} // namespace Bindings
} // namespace Storage
} // namespace Penjing
