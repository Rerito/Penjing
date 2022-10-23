// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include "../../Algorithm/MutatingNodeAlgorithm.hpp"
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
    constexpr auto operator()(
        Node& node,
        typename Node::StringViewType const& wordPath,
        typename Node::CharType expansion,
        NodeFactory&& makeNode) const
    {
        using std::begin;

        using std::empty;
        using std::size;

        if (empty(wordPath)) {
            return std::make_tuple(!!node[expansion], std::ref(node));
        }

        auto& t = mutableTransitionUnsafe(node, *begin(wordPath));

        auto const& label = t.label();

        // Since (node, wordPath) represents a state, wordPath is necessarily
        // strictly shorter than t's label.
        assert(size(wordPath) < size(label));
        if (*(begin(label) + size(wordPath)) == expansion) {
            return std::make_tuple(true, std::ref(node));
        }

        // Now a new node must be created to make the implicit state explicit
        // and allow branching from it.
        auto& newNode = Splitter(
            node,
            t,
            size(wordPath),
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
