// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <ranges>

#include <Penjing/Storage/Bindings/ArrayList.hpp>
#include <Penjing/Storage/Bindings/StdUnorderedMap.hpp>
#include <Penjing/Storage/Store.hpp>

#include "../Concepts/String.hpp"
#include "../Concepts/StringView.hpp"

#include "Node.hpp"

namespace Penjing {
namespace SuffixTree {

namespace Algorithm {

class MutatingTreeAlgorithm;

} // namespace Algorithm

namespace Core {

template<
    typename Str,
    typename StrView,
    typename NodeTraits =
        Storage::Bindings::StdUnorderedMap< std::allocator< Str > >,
    typename StorageTraits =
        Storage::Bindings::ArrayList< std::allocator< Str >, 64u > >
    requires Concepts::String< Str > && Concepts::StringView< Str, StrView >
class NakedTree
{
public:
    using CharType = std::ranges::range_value_t< Str >;
    using DiffType = std::ranges::range_difference_t< Str >;
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

    // TODO: Support actual removal
    constexpr void removeNode(NodeType& node) = delete;

    constexpr NodeType& mutableRoot() noexcept { return _root; }
};

} // namespace Core
} // namespace SuffixTree
} // namespace Penjing
