// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <ranges>

#include "../../Algorithm/MutatingNodeAlgorithm.hpp"
#include "../../Concepts/Node.hpp"
#include "../../Core/UnsafeTag.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Builders {
namespace Ukkonen {

namespace CPO {

template< typename Splitter >
class TestAndSplit
    : private Algorithm::MutatingNodeAlgorithm
    , private Splitter
{
private:
    using Splitter::split;

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
        auto& newNode = split(
            node,
            t,
            std::ranges::size(wordPath),
            std::forward< NodeFactory >(makeNode));

        return std::make_tuple(false, std::ref(newNode));
    }
};

} // namespace CPO

inline namespace Cust {

template< typename Splitter >
inline constexpr CPO::TestAndSplit< Splitter > testAndSplit{};

} // namespace Cust

template< typename Splitter >
struct TestAndSplitPolicy
{
    template< typename Node, typename NodeFactory >
    constexpr auto testAndSplit(
        Node& node,
        typename Node::StringViewType const& wordPath,
        typename Node::CharType expansion,
        NodeFactory&& makeNode) const
    {
        return Cust::testAndSplit< Splitter >(
            node,
            wordPath,
            expansion,
            std::forward< NodeFactory >(makeNode));
    }
};

} // namespace Ukkonen
} // namespace Builders
} // namespace SuffixTree
} // namespace Penjing
