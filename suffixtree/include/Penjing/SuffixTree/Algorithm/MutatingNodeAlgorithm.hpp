// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <utility>

#include "../Core/UnsafeTag.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Algorithm {

class MutatingNodeAlgorithm
{
protected:
    template< typename Node >
    constexpr auto mutableTransition(Node& node, typename Node::CharType start)
        const noexcept(noexcept(node.mutableTransition({}, std::move(start))))
    {
        return node.mutableTransition({}, std::move(start));
    }

    template< typename Node >
    constexpr auto& mutableTransitionUnsafe(
        Node& node,
        typename Node::CharType start) const
        noexcept(noexcept(
            node.mutableTransition({}, std::move(start), Core::UnsafeTag{})))
    {
        return node.mutableTransition({}, std::move(start), Core::UnsafeTag{});
    }

    template< typename Node >
    constexpr void setLink(Node& node, typename Node::NodePtr link) const
        noexcept(noexcept(node.setLink({}, std::move(link))))
    {
        node.setLink({}, std::move(link));
    }

    template< typename Node >
    constexpr auto addTransition(
        Node& node,
        typename Node::StringViewType label,
        typename Node::NodePtr target) const
        noexcept(noexcept(
            node.addTransition({}, std::move(label), std::move(target))))
    {
        return node.addTransition({}, std::move(label), std::move(target));
    }
};

} // namespace Algorithm
} // namespace SuffixTree
} // namespace Penjing
