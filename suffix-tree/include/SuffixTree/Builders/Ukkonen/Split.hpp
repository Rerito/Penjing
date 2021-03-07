// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <memory>
#include <ranges>

#include "../../Algorithm/MutatingNodeAlgorithm.hpp"

#include "../../Concepts/Node.hpp"

namespace SuffixTree {
namespace Builders {
namespace Ukkonen {

namespace CPO {

// /!\ Do not use Split outside of Ukkonen builder
class Split : private Algorithm::MutatingNodeAlgorithm
{
private:
    template< typename Node, typename NodeFactory >
    static constexpr bool _isNoExcept()
    {
        const bool noExceptCreation =
            noexcept(std::declval< NodeFactory >()(std::declval< Node* >()));

        const bool noExceptAddTransition =
            noexcept(std::declval< Split& >().mutableTransition(
                std::declval< Node& >(),
                std::declval< typename Node::CharType >()));

        return noExceptCreation && noExceptAddTransition;
    }

public:
    // Given a node N, a branch labeled t[1..M] to a node N2 and a splitting
    // point 0 < k < M, create a node N' and link it with N such that the
    // following tree structure is implemented:
    // N -- (t[0..k-1]) --> N' -- (t[k..M]) --> N2
    template< typename Node, typename NodeFactory >
        requires(Concepts::Node< Node >)
    constexpr auto& operator()(
        Node& node,
        typename Node::TransitionType& branch,
        std::ranges::range_difference_t< typename Node::StringViewType >
            branchingPoint,
        NodeFactory&& factory) const
        noexcept(_isNoExcept< Node, NodeFactory >())
    {
        using StrView = typename Node::StringViewType;

        assert(std::ranges::size(branch.label()) > branchingPoint);
        assert(0 < branchingPoint);

        auto& newNode = std::forward< NodeFactory >(factory)(&node);
        auto& label = branch.label();
        auto& target = branch.target();

        // Add the transition to the new node
        addTransition(
            newNode,
            StrView{
                std::ranges::begin(label) + branchingPoint,
                std::ranges::end(label)},
            target);

        // Now edit the existing transition so that it points to the new node
        // with a trimmed label.
        label = {
            std::ranges::begin(label),
            std::ranges::begin(label) + branchingPoint};
        target = std::addressof(newNode);

        return newNode;
    }
};

} // namespace CPO

inline namespace Cust {

inline constexpr CPO::Split split{};

} // namespace Cust

struct SplitPolicy
{
    template< typename Node, typename NodeFactory >
    constexpr decltype(auto) split(
        Node& node,
        typename Node::TransitionType& branch,
        std::ranges::range_difference_t< typename Node::StringViewType >
            branchingPoint,
        NodeFactory&& factory) const
        noexcept(noexcept(Cust::split(
            node,
            branch,
            branchingPoint,
            std::forward< NodeFactory >(factory))))
    {
        return Cust::split(
            node,
            branch,
            branchingPoint,
            std::forward< NodeFactory >(factory));
    }
};

} // namespace Ukkonen
} // namespace Builders
} // namesapce SuffixTree
