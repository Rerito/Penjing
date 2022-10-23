// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <functional>
#include <queue>

#include <Penjing/Meta/MakeIteratorRange.hpp>

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
    constexpr bool
    operator()(Node1 const& lhs, Node2 const& rhs, Compare&& compare) const
        noexcept(_isNoExcept< Node1, Node2, Compare& >())
    {
        using std::begin;
        using std::end;

        using std::empty;
        using std::mismatch;
        using std::size;

        using NodeQueue = typename QueueTraits::template Type<
            std::pair< Node1 const&, Node2 const& > >;

        NodeQueue queue;
        queue.emplace(std::make_pair(std::cref(lhs), std::cref(rhs)));

        while (!empty(queue)) {
            auto const& [n1, n2] = queue.front();

            if (n1.childrenCount() != n2.childrenCount()) {
                return false;
            }

            auto n1Transitions = n1.transitions();

            for (auto const& t1 : Meta::makeIteratorRange(n1Transitions)) {
                auto const& t1Label = t1.second.label();
                auto t2 = n2.transition(*begin(t1Label));

                // There is no equivalent transition in n2
                if (!t2) {
                    return false;
                }

                auto const& t2Label = (*t2).get().label();

                // The transition labels have mismatched lengths: they can't be
                // equivalent.
                if (size(t1Label) != size(t2Label)) {
                    return false;
                }

                auto const labelCompare = mismatch(
                    begin(t1Label),
                    end(t1Label),
                    begin(t2Label),
                    end(t2Label),
                    compare);

                // A mismatch occurred before the end of the labels comparison
                if (end(t1Label) != labelCompare.first ||
                    end(t2Label) != labelCompare.second) {
                    return false;
                }

                queue.emplace(std::make_pair(
                    std::cref(*(t1.second)),
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
