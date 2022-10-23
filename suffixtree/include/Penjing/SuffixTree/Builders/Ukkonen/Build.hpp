// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <Penjing/Meta/DifferenceType.hpp>

#include "../../Algorithm/MutatingNodeAlgorithm.hpp"
#include "../../Algorithm/Walk.hpp"

#include "Canonize.hpp"
#include "Update.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Builders {
namespace Ukkonen {

namespace CPO {

template< auto Canonize = Cust::canonize, auto Update = Cust::update<> >
class Build : public Algorithm::MutatingNodeAlgorithm
{
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
        using std::begin;
        using std::end;

        using std::size;

        using StrView = typename Node::StringViewType;
        using DiffType = Meta::DifferenceType< StrView >;

        auto [initialActiveNode, toInsert, currentExpansion] =
            fetchActivePoint(root, word);

        auto activeNode = std::ref(initialActiveNode);

        while (size(toInsert) > currentExpansion) {
            StrView label{};

            // label is the proper view on the string for the next insertion
            // However it still needs to be canonized
            std::tie(activeNode, label) = Update(
                root,
                activeNode.get(),
                toInsert,
                currentExpansion,
                makeNode);

            auto [canonizedActiveNode, canonizedLabel] =
                Canonize(activeNode.get(), label);
            label = std::move(canonizedLabel);

            // To be able to properly perform the next insertion, we have to
            // suffix the label view with the remainder of the toInsert view.
            // This is possible because the implementation guarantees that label
            // and toInsert are contiguous views over the same string.
            toInsert = {begin(label), end(toInsert)};

            // The current expansion for the next update call is marked by the
            // end of the label view.
            currentExpansion = size(label);
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

template< auto Canonize = canonize, auto Update = update<> >
inline constexpr CPO::Build< Canonize, Update > build{};

} // namespace Cust

} // namespace Ukkonen

// Convenience alias for the Ukkonen suffix tree builder.
inline constexpr auto ukkonen = Ukkonen::build<>;

} // namespace Builders
} // namespace SuffixTree
} // namespace Penjing
