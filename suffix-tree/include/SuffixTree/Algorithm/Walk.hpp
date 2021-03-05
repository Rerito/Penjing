// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT
#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <ranges>
#include <tuple>
#include <utility>

#include "../Concepts/Node.hpp"
#include "../Concepts/StringView.hpp"

namespace SuffixTree {
namespace Algorithm {

namespace CPO {

class Walk
{
private:
    template< typename Root, typename StrView >
    static constexpr bool _isNoExcept()
    {
        return noexcept(std::declval< Root >().operator[](
            std::declval< std::ranges::range_value_t< StrView > >()));
    }

public:
    template< typename Node, typename StrView >
        requires Concepts::Node< Node > &&
            Concepts::StringView< typename Node::StringType, StrView > &&
            std::convertible_to<
                typename Node::CharType,
                std::ranges::range_value_t< StrView > >
    constexpr auto operator()(Node const& root, StrView word) const
    {
        auto matchingNode = std::cref(root);
        std::ranges::range_difference_t< StrView > matchingSize = 0;

        while (!std::ranges::empty(word)) {
            auto const transition =
                matchingNode.get()[*std::ranges::begin(word)];

            // If there is no transition, it's a mismatch: the given word is
            // matched up to the explicit state (matchingNode, {}).
            if (!transition) {
                matchingSize = 0;
                break;
            }

            auto const& label = (*transition).get().label();

            // Compare the transition label with the remainder of the word to
            // match...
            auto mismatched = std::ranges::mismatch(label, word);
            matchingSize = mismatched.in1 - std::ranges::begin(label);

            // ... If a mismatch occured before the end of the label, the given
            // word is matched up to the implicit state:
            // (
            //     matchingNode,
            //     matchingLabel={std::ranges::begin(label), mismatched.in1}
            // ).
            if (std::ranges::size(label) != matchingSize) {
                break;
            }

            word = StrView(mismatched.in2, std::ranges::end(word));

            // Otherwise the target of the matching transition becomes the basis
            // for the walk.
            matchingNode = std::cref(*((*transition).get()));
        }

        return std::make_tuple(matchingNode, word, matchingSize);
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::Walk walk{};

} // namespace Cust

} // namespace Algorithm
} // namespace SuffixTree
