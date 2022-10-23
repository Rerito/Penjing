// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <iostream>
#include <string>

#include <Penjing/Meta/MakeIteratorRange.hpp>

#include <Penjing/SuffixTree/Concepts/Node.hpp>

namespace Penjing {
namespace SuffixTree {
namespace Test {

class Dump
{
private:
    template< typename Node >
        requires(Concepts::Node< Node >)
    constexpr void dump(Node const& node, std::ostream& ostream, size_t offset)
        const
    {
        std::string whitespace(offset, ' ');
        auto transitions = node.transitions();
        for (auto const& t : Meta::makeIteratorRange(transitions)) {
            ostream << whitespace << "\\--[" << t.second.label() << "]-->o"
                    << std::endl;
            dump(
                *(t.second),
                ostream,
                offset + 8 + std::ranges::size(t.second.label()));
        }
    }

public:
    template< typename Node >
    constexpr void operator()(Node const& node, std::ostream& ostream) const
    {
        ostream << 'o' << std::endl;
        dump(node, ostream, 0u);
    }
};

inline namespace Cust {

inline constexpr Dump dump{};

} // namespace Cust

} // namespace Test
} // namespace SuffixTree
} // namespace Penjing
