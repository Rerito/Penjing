// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <memory>
#include <ranges>
#include <type_traits>

#include <Penjing/Meta/IsDetected.hpp>
#include <Penjing/Meta/SelfVariant.hpp>
#include <Penjing/Meta/VariantHelpers.hpp>

namespace Penjing {
namespace SuffixTree {
namespace Core {

namespace Traits {

// Convenience alias for the given `StorageTraits` `StorageType` template alias.
template< typename StorageTraits, typename Node >
using StorageType = typename StorageTraits::template StorageType< Node >;

template< typename StorageTraits, typename Node >
using NodeRemovalType = decltype(StorageTraits::remove(
    std::declval<
        std::add_lvalue_reference_t< StorageType< StorageTraits, Node > > >(),
    std::declval< std::add_lvalue_reference_t< Node > >()));

template< typename StorageTraits, typename Storage, typename... Args >
using NodeEmplaceType = decltype((StorageTraits::emplace(
    std::declval< std::add_lvalue_reference_t< Storage > >(),
    std::declval< Args >()...)));

template< typename StorageTraits, typename Node >
using HasNodeRemoval = Meta::IsDetected< NodeRemovalType, StorageTraits, Node >;

template< typename Node, bool HasNativeRemoval >
struct NodeElement
{
    using Type = Node;
};

template< typename Node >
    requires Meta::NoStorageOffsetVariant<
        Meta::SelfVariant< std::monostate, Node* > >
struct NodeElement< Node, false >
{
    using Type = Meta::SelfVariant< std::monostate, Node >;
};

} // namespace Traits

// Companion class that handles the storage of tree nodes.
// The template expects two type parameters:
//  - Node The Node type to store.
//  - StorageTraits The traits class defining the storage behavior.
// StorageTraits must have the inner member template alias StorageType and the
// member function `emplace`.
// Additionally, if the `remove` function is defined and is callable with a
// `Node` argument, it will be used as the node removal policy.
template< typename Node, typename StorageTraits >
class NodeStore
{
private:
    static constexpr bool _hasNativeRemoval()
    {
        return Traits::HasNodeRemoval< StorageTraits, Node >::value;
    }

public:
    // If the StorageTraits does not define a node removal policy,
    // we fallback on a free-list policy to try to maximize cache spatial
    // locality.
    using NodeElement =
        typename Traits::NodeElement< Node, _hasNativeRemoval() >::Type;

    using NodeStorage =
        typename StorageTraits::template StorageType< NodeElement >;

private:
    NodeStorage _storage;
    NodeElement* _free;
    size_t _activeNodesCount;
    size_t _freeNodeCount;

private:
    static constexpr bool _hasNoExceptRemoval()
    {
        if constexpr (_hasNativeRemoval()) {
            return noexcept(StorageTraits::remove(
                std::declval< NodeStorage& >(),
                std::declval< Node&& >()));
        } else {
            return std::is_nothrow_assignable_v< NodeElement, NodeElement* >;
        }
    }

    template< typename... Args >
    static constexpr bool _canEmplace()
    {
        return std::is_convertible_v<
            Traits::NodeEmplaceType< StorageTraits, NodeStorage, Args... >,
            std::add_lvalue_reference_t< NodeElement > >;
    }

    template< typename... Args >
    static constexpr bool _hasNoExceptEmplace()
    {
        return noexcept(StorageTraits::emplace(
            std::declval< NodeStorage& >(),
            std::declval< Args >()...));
    }

public:
    template< typename... Args >
        requires(_canEmplace< Args... >() && _hasNativeRemoval())
    Node& addNode(Args&&... args) noexcept(_hasNoExceptEmplace< Args&&... >())
    {
        auto& node =
            StorageTraits::emplace(_storage, std::forward< Args >(args)...);
        ++_activeNodesCount;

        return node;
    }

    template< typename... Args >
        requires(_canEmplace< Node&& >())
    Node& addNode(Args&&... args) noexcept(_hasNoExceptEmplace< Node&& >())
    {
        NodeElement* nodeVariant = nullptr;
        if (nullptr != _free) {
            nodeVariant = _free;
            auto node = Node{std::forward< Args >(args)...};

            // Cannot throw as the _free pointer points to a recycled node.
            _free = std::get< NodeElement* >(*nodeVariant);

            *nodeVariant = std::move(node);
            --_freeNodeCount;
        } else {
            nodeVariant = std::addressof(StorageTraits::emplace(
                _storage,
                Node{std::forward< Args >(args)...}));
        }

        ++_activeNodesCount;

        assert(nullptr != nodeVariant);
        // This cannot throw as the variant is currently storing the node.
        return std::get< Node >(*nodeVariant);
    }

    void removeNode(Node&& node) noexcept(_hasNoExceptRemoval()) requires(
        _hasNativeRemoval())
    {
        // The move is used to signal the caller that the object should not be
        // used after the remove.
        StorageTraits::remove(_storage, std::move(node));
        --_activeNodesCount;
    }

    void removeNode(Node&& node) noexcept(_hasNoExceptRemoval())
    {
        // 1. From the node's reference, deduce the containing variant.
        auto& asVariant = *static_cast< NodeElement* >(
            static_cast< void* >(std::addressof(node)));

        // 2. Destroy the node object and update the freelist.
        asVariant = _free;
        _free = std::addressof(asVariant);
        ++_freeNodeCount;
        --_activeNodesCount;
    }

private:
    static constexpr Node& nodeFromElement(NodeElement& element)
    {
        return std::get< Node >(element);
    }

    static constexpr Node const& nodeFromElement(NodeElement const& element)
    {
        return std::get< Node >(element);
    }
};

} // namespace Core
} // namespace SuffixTree
} // namespace Penjing
