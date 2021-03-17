// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <functional>
#include <ranges>
#include <tuple>

#include "../../Concepts/Node.hpp"
#include "../../Concepts/StringView.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Builders {
namespace Ukkonen {

namespace CPO {

// /!\ Warning: do not use outside of the Ukkonen builder
class Canonize
{
public:
    // This function is always called with a wordPath related to the string
    // being added. The canonical path is sure to exist thanks to
    // "open transitions" left during the tree construction.
    template< typename Node, typename StrView >
        requires(Concepts::Node< Node >&&
                     Concepts::StringView< typename Node::StringType, StrView >)
    constexpr auto operator()(Node const& node, StrView wordPath) const
    {
        auto canonicalNode = std::cref(node);

        // If the wordPath to follow is empty, the state is explicit
        // and therefore already canonical.
        if (std::ranges::empty(wordPath)) {
            return std::make_tuple(canonicalNode, wordPath);
        }

        // Otherwise, wordPath must be fast-walked to canonize the given state.
        auto t = canonicalNode.get()[*std::ranges::begin(wordPath)];
        assert(!!t);

        // As long as there is a transition with a matching begin, wordPath and
        // its label are sure to match up until:
        // min(size(wordPath), size(label)).
        // This is the "fast" walk, as there is no need to compare characters.
        while (std::ranges::size((*t).get().label()) <=
               std::ranges::size(wordPath)) {

            // Skim wordPath at the beginning of size(label) characters to
            // prepare for the next iteration
            wordPath = {
                std::ranges::begin(wordPath) +
                    std::ranges::size((*t).get().label()),
                std::ranges::end(wordPath)};

            canonicalNode = std::cref(*((*t).get()));

            // The wordPath has been fully walked down
            if (std::ranges::empty(wordPath)) {
                break;
            }

            // Otherwise, there is necessarily an existing transition that
            // shares a common prefix with wordPath
            t = canonicalNode.get()[*std::ranges::begin(wordPath)];
            assert(!!t);
        }

        return std::make_tuple(canonicalNode, wordPath);
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::Canonize canonize{};

} // namespace Cust

struct CanonizePolicy
{
    template< typename Node, typename StrView >
    constexpr auto canonize(Node const& node, StrView&& wordPath) const
        noexcept(
            noexcept(Cust::canonize(node, std::forward< StrView >(wordPath))))
    {
        return Cust::canonize(node, std::forward< StrView >(wordPath));
    }
};

} // namespace Ukkonen
} // namespace Builders
} // namesapce SuffixTree
} // namespace Penjing
