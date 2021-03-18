// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include "../ArrayList.hpp"

namespace Penjing {
namespace Storage {
namespace Bindings {

template< typename Alloc, size_t ArraySize >
struct ArrayList
{
    using AllocatorType = Alloc;

    template< typename Value >
    using StorageType = Storage::ArrayList< Value, Alloc, ArraySize >;

    template< typename Value, typename... Args >
    static Value& emplace(
        Storage::ArrayList< Value, Alloc, ArraySize >& storage,
        Args&&... args) noexcept(noexcept(storage
                                              .emplace(
                                                  std::declval< Args&& >()...)))
    {
        return storage.emplace(std::forward< Args >(args)...);
    }
};

} // namespace Bindings
} // namespace Storage
} // namespace Penjing
