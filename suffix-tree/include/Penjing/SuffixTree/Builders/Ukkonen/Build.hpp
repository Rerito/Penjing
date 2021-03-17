// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include "../../Algorithm/MutatingNodeAlgorithm.hpp"
#include "../../Algorithm/Walk.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Builders {
namespace Ukkonen {

namespace CPO {

template< typename Canonizer, typename Updater >
class Build
    : private Algorithm::MutatingNodeAlgorithm
    , private Canonizer
    , private Updater
{
private:
    using Canonizer::canonize;
    using Updater::update;

private:
    // TODO: Write proper noexcept specifier
    template< typename Node, typename NodeFactory >
    static constexpr bool _isNoExcept()
    {
        return false;
    }

public:
    template< typename Node, typename NodeFactory >
    constexpr void operator()(
        Node& root,
        typename Node::StringViewType word,
        NodeFactory&& makeNode) const
        noexcept(_isNoExcept< Node, NodeFactory&& >())
    {
        using StrView = typename Node::StringViewType;
        using DiffType = std::ranges::range_difference_t< StrView >;

        auto [initialActiveNode, toInsert, currentExpansion] =
            fetchActivePoint(root, word);

        auto activeNode = std::ref(initialActiveNode);

        while (std::ranges::size(toInsert) > currentExpansion) {
            StrView label{};

            // label is the proper view on the string for the next insertion
            // However it still needs to be canonized
            std::tie(activeNode, label) = update(
                root,
                activeNode.get(),
                toInsert,
                currentExpansion,
                makeNode);

            auto [canonizedActiveNode, canonizedLabel] =
                canonize(activeNode.get(), label);
            label = std::move(canonizedLabel);

            // To be able to properly perform the next insertion, we have to
            // suffix the label view with the remainder of the toInsert view.
            // This is possible because the implementation guarantees that label
            // and toInsert are contiguous views over the same string.
            toInsert = {std::ranges::begin(label), std::ranges::end(toInsert)};

            // The current expansion for the next update call is marked by the
            // end of the label view.
            currentExpansion = std::ranges::size(label);
            activeNode = std::ref(const_cast< Node& >(canonizedActiveNode));
        }
    }

private:
    template< typename Node >
    constexpr auto fetchActivePoint(
        Node& root,
        typename Node::StringViewType word) const
    {
        // Fetch the active point
        auto [activeNode, toInsert, expansion] = Algorithm::walk(root, word);

        return std::make_tuple(
            std::ref(const_cast< Node& >(activeNode)),
            std::move(toInsert),
            std::move(expansion));
    }
};

} // namespace CPO

inline namespace Cust {

template< typename Canonizer, typename Updater >
inline constexpr CPO::Build< Canonizer, Updater > build{};

} // namespace Cust

} // namespace Ukkonen
} // namespace Builders
} // namespace SuffixTree
} // namespace Penjing
