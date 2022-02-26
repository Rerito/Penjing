// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

namespace Penjing {
namespace SuffixTree {
namespace Algorithm {

class MutatingTreeAlgorithm
{
protected:
    template< typename Tree, typename... Args >
    constexpr decltype(auto) addNode(Tree& tree, Args&&... args) const noexcept
    {
        return tree.addNode({}, std::forward< Args >(args)...);
    }

    template< typename Tree >
    constexpr decltype(auto) mutableRoot(Tree& tree) const noexcept
    {
        return tree.mutableRoot({});
    }

    template< typename Tree >
    constexpr auto nodeFactory(Tree& tree) const noexcept
    {
        return tree.nodeFactory({});
    }

    template< typename Tree >
    constexpr decltype(auto) mutableStringStorage(Tree& tree) const noexcept
    {
        return tree.mutableStringStorage({});
    }
};

} // namespace Algorithm
} // namespace SuffixTree
} // namespace Penjing
