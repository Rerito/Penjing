// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <queue>

#include <Penjing/SuffixTree/Concepts/Node.hpp>

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
            for (auto t : transitions) {
                ostream << "[" << t.get().label() << "]";
                toVisit.emplace(std::cref(*t.get().target()));
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
