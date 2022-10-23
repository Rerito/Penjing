// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <queue>

#include <Penjing/Meta/MakeIteratorRange.hpp>

namespace Penjing {
namespace SuffixTree {
namespace Test {

class ToStream
{
public:
    template< typename Node, typename OStream >
    constexpr void operator()(Node const& node, OStream& ostream) const
    {
        using NodeRefW = decltype(std::cref(node));
        std::queue< NodeRefW > toVisit;
        toVisit.push(std::cref(node));

        while (!toVisit.empty()) {
            auto const& currentNode = toVisit.front().get();
            toVisit.pop();

            auto transitions = currentNode.transitions();
            for (auto const& t : Meta::makeIteratorRange(transitions)) {
                ostream << "[" << t.second.label() << "]";
                toVisit.emplace(std::cref(*t.second.target()));
            }
        }
    }
};

inline namespace Cust {

inline constexpr ToStream toStream{};

} // namespace Cust

} // namespace Test
} // namespace SuffixTree
} // namespace Penjing
