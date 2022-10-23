// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include "../Algorithm/MutatingTreeAlgorithm.hpp"

#include "../Concepts/HasStringStorage.hpp"

namespace Penjing {
namespace SuffixTree {

namespace CPO {

class TreeBuild : public Algorithm::MutatingTreeAlgorithm
{
private:
    template< typename Tree, typename... Args >
        requires(Concepts::HasStringStorage< Tree >)
    constexpr auto viewToInsert(Tree& tree, Args&&... args) const
    {
        using std::begin;
        using std::end;
        using StrView = typename Tree::StringViewType;
        auto const& newString =
            mutableStringStorage(tree).emplace(std::forward< Args >(args)...);

        return StrView{begin(newString), end(newString)};
    }

    template< typename Tree >
    constexpr auto viewToInsert(Tree& tree, typename Tree::StringViewType word)
        const noexcept
    {
        return word;
    }

public:
    template< typename Builder, typename Tree, typename... Args >
    constexpr void operator()(Builder&& build, Tree& tree, Args&&... args) const
    {
        std::forward< Builder >(build)(
            mutableRoot(tree),
            viewToInsert(tree, std::forward< Args >(args)...),
            nodeFactory(tree));
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::TreeBuild build = {};

} // namespace Cust

} // namespace SuffixTree
} // namespace Penjing
