// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>

#include <Penjing/Meta/DifferenceType.hpp>
#include <Penjing/Meta/ValueType.hpp>

#include <Penjing/Storage/Bindings/ArrayList.hpp>
#include <Penjing/Storage/Bindings/StdUnorderedMap.hpp>
#include <Penjing/Storage/Store.hpp>

#include "Node.hpp"

namespace Penjing {
namespace SuffixTree {

namespace Algorithm {

class MutatingTreeAlgorithm;

} // namespace Algorithm

namespace Core {

// A NakedTree is a suffix tree that only manages its own nodes.
// It works with string views and assumes that the views will remain valid
// during the tree lifecycle.
template<
    typename Str,
    typename StrView,
    typename NodeTraits =
        Storage::Bindings::StdUnorderedMap< std::allocator< Str > >,
    typename StorageTraits =
        Storage::Bindings::ArrayList< std::allocator< Str >, 64u > >
class NakedTree
{
public:
    using StringType = Str;
    using StringViewType = StrView;
    using CharType = Meta::ValueType< Str >;
    using DiffType = Meta::DifferenceType< Str >;
    using NodeType = Node< Str, StrView, NodeTraits >;
    using NodeAllocator =
        std::allocator_traits< typename StorageTraits::AllocatorType >::
            template rebind_alloc< NodeType >;

    using NodeStorage = Storage::Store< NodeType, StorageTraits >;

private:
    NodeStorage _storage;
    NodeType _root;

public:
    template< typename... Args >
    constexpr decltype(auto) addNode(Args&&... args) noexcept(
        noexcept(this->_storage.emplace(std::declval< Args&& >()...)))
    {
        return _storage.emplace(std::forward< Args >(args)...);
    }

    constexpr NodeType& mutableRoot() noexcept { return _root; }
    constexpr NodeType const& root() const noexcept { return _root; }

    constexpr auto nodeFactory()
    {
        return [this](auto&&... args) -> decltype(auto) {
            return addNode(std::forward< decltype(args) >(args)...);
        };
    }
};

} // namespace Core
} // namespace SuffixTree
} // namespace Penjing
