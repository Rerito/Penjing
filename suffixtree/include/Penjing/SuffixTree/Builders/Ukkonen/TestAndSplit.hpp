// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <ranges>

#include "../../Algorithm/MutatingNodeAlgorithm.hpp"
#include "../../Concepts/Node.hpp"
#include "../../Core/UnsafeTag.hpp"

#include "Split.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Builders {
namespace Ukkonen {

namespace CPO {

template< auto Splitter = Cust::split >
class TestAndSplit : public Algorithm::MutatingNodeAlgorithm
{
public:
    template< typename Node, typename NodeFactory >
        requires(Concepts::Node< Node >)
    constexpr auto operator()(
        Node& node,
        typename Node::StringViewType const& wordPath,
        typename Node::CharType expansion,
        NodeFactory&& makeNode) const
    {
        if (std::ranges::empty(wordPath)) {
            return std::make_tuple(!!node[expansion], std::ref(node));
        }

        auto& t = mutableTransitionUnsafe(node, *std::ranges::begin(wordPath));

        auto const& label = t.label();

        // Since (node, wordPath) represents a state, wordPath is necessarily
        // strictly shorter than t's label.
        assert(std::ranges::size(wordPath) < std::ranges::size(label));
        if (*(std::ranges::begin(label) + std::ranges::size(wordPath)) ==
            expansion) {
            return std::make_tuple(true, std::ref(node));
        }

        // Now a new node must be created to make the implicit state explicit
        // and allow branching from it.
        auto& newNode = Splitter(
            node,
            t,
            std::ranges::size(wordPath),
            std::forward< NodeFactory >(makeNode));

        return std::make_tuple(false, std::ref(newNode));
    }
};

} // namespace CPO

inline namespace Cust {

template< auto Splitter = split >
inline constexpr CPO::TestAndSplit< Splitter > testAndSplit{};

} // namespace Cust

} // namespace Ukkonen
} // namespace Builders
} // namespace SuffixTree
} // namespace Penjing
