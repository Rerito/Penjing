// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

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
};

} // namespace Algorithm
} // namespace SuffixTree
