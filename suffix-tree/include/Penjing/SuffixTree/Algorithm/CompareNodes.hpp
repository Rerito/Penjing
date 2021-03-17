// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <functional>
#include <queue>
#include <ranges>

#include "../Concepts/Node.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Algorithm {

namespace Details {

struct StdQueueTraits
{
    template< typename Element >
    using Type = std::queue< Element >;
};

} // namespace Details

namespace CPO {

template< typename QueueTraits >
class CompareNodes
{
private:
    template< typename Node1, typename Node2, typename Compare >
    static constexpr bool _isNoExcept()
    {
        using NodeRefPair = std::pair< Node1 const&, Node2 const& >;
        using NodeQueue = typename QueueTraits::template Type< NodeRefPair >;

        // Queue related noexcept specifications
        constexpr bool noExceptEmplace =
            noexcept(std::declval< NodeQueue& >().emplace(
                std::declval< NodeRefPair >()));

        constexpr bool noExceptPop =
            noexcept(std::declval< NodeQueue& >().pop());

        // Comparison noexcept specification
        constexpr bool noExceptCompare =
            noexcept(std::declval< Compare >().operator()(
                std::declval< typename Node1::CharType >(),
                std::declval< typename Node2::CharType >()));

        // Node access noexcept specifications
        if constexpr (
            !noexcept(std::declval< Node1 const& >().transitions()) ||
            !noexcept(std::declval< Node2 const& >().transition(
                std::declval< typename Node1::CharType >()))) {
            return false;
        }

        return noExceptEmplace && noExceptPop && noExceptCompare;
    }

public:
    template< typename Node1, typename Node2, typename Compare >
        requires(Concepts::Node< Node1 >&& Concepts::Node< Node2 >&&
                     std::convertible_to<
                         typename Node1::CharType,
                         typename Node2::CharType >&&
                         std::convertible_to<
                             std::invoke_result_t<
                                 Compare&&,
                                 typename Node1::CharType,
                                 typename Node2::CharType >,
                             bool >)
    constexpr bool
    operator()(Node1 const& lhs, Node2 const& rhs, Compare&& compare) const
        noexcept(_isNoExcept< Node1, Node2, Compare& >())
    {
        using NodeQueue = typename QueueTraits::template Type<
            std::pair< Node1 const&, Node2 const& > >;

        NodeQueue queue;
        queue.emplace(std::make_pair(std::cref(lhs), std::cref(rhs)));

        while (!std::ranges::empty(queue)) {
            auto const& [n1, n2] = queue.front();

            if(n1.childrenCount() != n2.childrenCount()) {
                return false;
            }

            auto n1Transitions = n1.transitions();

            for (auto const& t1 : n1Transitions) {
                auto const& t1Label = t1.get().label();
                auto t2 = n2.transition(*std::ranges::begin(t1Label));

                // There is no equivalent transition in n2
                if (!t2) {
                    return false;
                }

                auto const& t2Label = (*t2).get().label();

                // The transition labels have mismatched lengths: they can't be
                // equivalent.
                if (std::ranges::size(t1Label) != std::ranges::size(t2Label)) {
                    return false;
                }

                auto const labelCompare =
                    std::ranges::mismatch(t1Label, t2Label, compare);

                // A mismatch occurred before the end of the labels comparison
                if (std::ranges::end(t1Label) != labelCompare.in1 ||
                    std::ranges::end(t2Label) != labelCompare.in2) {
                    return false;
                }

                queue.emplace(std::make_pair(
                    std::cref(*(t1.get())),
                    std::cref(*((*t2).get()))));
            }

            queue.pop();
        }

        return true;
    }

    template< typename Node1, typename Node2 >
    constexpr bool operator()(Node1 const& lhs, Node2 const& rhs) const
        noexcept(noexcept(operator()(lhs, rhs, std::equal_to<>{})))
    {
        return operator()(lhs, rhs, std::equal_to<>{});
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::CompareNodes< Details::StdQueueTraits > compareNodes{};

} // namespace Cust

} // namespace Algorithm
} // namespace SuffixTree
} // namespace Penjing
