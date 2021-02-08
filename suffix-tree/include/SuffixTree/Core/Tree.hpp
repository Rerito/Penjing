// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <ranges>

#include "../Concepts/NodeTraits.hpp"
#include "../Concepts/String.hpp"
#include "../Concepts/StringView.hpp"

#include "ArrayListStorage.hpp"
#include "Node.hpp"
#include "NodeStore.hpp"
#include "StdUnorderedMapNodeTraits.hpp"

namespace SuffixTree {

namespace Algorithm {

class MutatingTreeAlgorithm;

} // namespace Algorithm

namespace Core {

template<
    typename Str,
    typename StrView,
    typename NodeTraits = StdUnorderedMapNodeTraits< std::allocator< Str > >,
    typename StorageTraits =
        ArrayListStorageTraits< std::allocator< Str >, 64u > >
    requires Concepts::String< Str > && Concepts::StringView< Str, StrView > &&
        Concepts::NodeTraits< NodeTraits >
class Tree
{
public:
    using CharType = std::ranges::range_value_t< Str >;
    using DiffType = std::ranges::range_difference_t< Str >;
    using NodeType = Node< Str, StrView, NodeTraits >;
    using NodeAllocator =
        std::allocator_traits< typename StorageTraits::AllocatorType >::
            template rebind_alloc< NodeType >;

    using NodeStorage = NodeStore< NodeType, StorageTraits >;

private:
    NodeStorage _storage;
    NodeType _root;

public:
    template< typename... Args >
    constexpr decltype(auto) addNode(
        Meta::Access< Algorithm::MutatingTreeAlgorithm >,
        Args&&... args) noexcept(noexcept(this->_storage
                                              .addNode(
                                                  std::declval< Args&& >()...)))
    {
        return _storage.addNode(std::forward< Args >(args)...);
    }

    // TODO: Support actual removal
    constexpr void removeNode(
        Meta::Access< Algorithm::MutatingTreeAlgorithm >,
        NodeType& node) = delete;
};

} // namespace Core
} // namespace SuffixTree
