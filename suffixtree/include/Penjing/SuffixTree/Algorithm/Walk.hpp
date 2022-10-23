// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT
#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <tuple>
#include <utility>

#include <Penjing/Meta/DifferenceType.hpp>
#include <Penjing/Meta/ValueType.hpp>

namespace Penjing {
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
            std::declval< Meta::ValueType< StrView > >()));
    }

public:
    template< typename Node, typename StrView >
    constexpr auto operator()(Node const& root, StrView word) const
    {
        using std::begin;
        using std::empty;
        using std::end;
        using std::mismatch;
        using std::size;

        auto matchingNode = std::cref(root);
        Meta::DifferenceType< StrView > matchingSize = 0;

        while (!empty(word)) {
            auto const transition = matchingNode.get()[*begin(word)];

            // If there is no transition, it's a mismatch: the given word is
            // matched up to the explicit state (matchingNode, {}).
            if (!transition) {
                matchingSize = 0;
                break;
            }

            auto const& label = (*transition).get().label();

            // Compare the transition label with the remainder of the word to
            // match...
            auto mismatched =
                mismatch(begin(label), end(label), begin(word), end(word));
            matchingSize = mismatched.first - begin(label);

            // ... If a mismatch occured before the end of the label, the given
            // word is matched up to the implicit state:
            // (
            //     matchingNode,
            //     matchingLabel={begin(label), mismatched.in1}
            // ).
            if (size(label) != matchingSize) {
                break;
            }

            word = StrView(mismatched.second, end(word));

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
} // namespace Penjing
