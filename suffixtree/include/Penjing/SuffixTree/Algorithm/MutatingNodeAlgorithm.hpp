// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <utility>

#include <Penjing/Meta/Access.hpp>

#include "../Core/UnsafeTag.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Algorithm {

class MutatingNodeAlgorithm
{
private:
    using AccessType = Meta::Access< MutatingNodeAlgorithm >;

protected:
    template< typename Node >
    constexpr auto mutableTransition(Node& node, typename Node::CharType start)
        const noexcept(noexcept(node.mutableTransition({}, std::move(start))))
    {
        return node.mutableTransition(AccessType{}, std::move(start));
    }

    template< typename Node >
    constexpr auto& mutableTransitionUnsafe(
        Node& node,
        typename Node::CharType start) const
        noexcept(noexcept(node.mutableTransition(
            AccessType{},
            std::move(start),
            Core::UnsafeTag{})))
    {
        return node.mutableTransition(
            AccessType{},
            std::move(start),
            Core::UnsafeTag{});
    }

    template< typename Node >
    constexpr void setLink(Node& node, typename Node::NodePtr link) const
        noexcept(noexcept(node.setLink({}, std::move(link))))
    {
        node.setLink(AccessType{}, std::move(link));
    }

    template< typename Node >
    constexpr auto addTransition(
        Node& node,
        typename Node::StringViewType label,
        typename Node::NodePtr target) const
        noexcept(noexcept(node.addTransition(
            AccessType{},
            std::move(label),
            std::move(target))))
    {
        return node.addTransition(
            AccessType{},
            std::move(label),
            std::move(target));
    }
};

} // namespace Algorithm
} // namespace SuffixTree
} // namespace Penjing
