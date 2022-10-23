// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>

#include <Penjing/Meta/DifferenceType.hpp>

#include "../../Algorithm/MutatingNodeAlgorithm.hpp"

#include "Canonize.hpp"
#include "TestAndSplit.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Builders {
namespace Ukkonen {

namespace CPO {

template<
    auto Canonizer = Cust::canonize,
    auto TestAndSplitter = Cust::testAndSplit<> >
class Update : public Algorithm::MutatingNodeAlgorithm
{
public:
    template< typename Node, typename NodeFactory >
    constexpr auto operator()(
        Node& root,
        Node& node,
        typename Node::StringViewType const& wordPath,
        Meta::DifferenceType< typename Node::StringViewType > currentExpansion,
        NodeFactory&& makeNode) const
    {
        using std::begin;
        using std::end;

        using StrView = typename Node::StringViewType;

        bool advanceOnCanonize = false;
        auto oldRef = std::ref(root);
        auto currentNode = std::ref(node);
        auto currentRef = std::ref(node);
        bool isEndPoint = true;

        auto expansionIt = (begin(wordPath) + currentExpansion);
        StrView expandingBranch{begin(wordPath), expansionIt};

        auto const expansionChar = *expansionIt;

        std::tie(isEndPoint, currentRef) =
            TestAndSplitter(node, expandingBranch, expansionChar, makeNode);

        while (!isEndPoint) {
            // currentRef points to the newly created branching state, ready to
            // host the new branch.
            auto& newState = makeNode(std::addressof(currentRef.get()));

            // Add that new branch to currentRef.
            addTransition(
                currentRef.get(),
                StrView{expansionIt, end(wordPath)},
                std::addressof(newState));

            // Update the suffix link of the previous expansion reference node.
            if (std::addressof(oldRef.get()) != std::addressof(root)) {
                setLink(oldRef.get(), std::addressof(currentRef.get()));
            }

            oldRef = currentRef;

            // Follow the link if it exists and canonize the state defined by
            // the pair: (link(currentNode), expandingBranch)
            std::tie(currentNode, expandingBranch) = canonizeLink(
                root,
                currentNode.get(),
                expandingBranch,
                advanceOnCanonize);

            // When `advanceOnCanonize` becomes true, the end point is reached
            // so testAndSplit will necessarily returns true and no node will
            // be created.
            // Proceed for the next expansion round...
            std::tie(isEndPoint, currentRef) = TestAndSplitter(
                currentNode.get(),
                expandingBranch,
                expansionChar,
                makeNode);
        }

        if (std::addressof(oldRef.get()) != std::addressof(root)) {
            setLink(oldRef.get(), std::addressof(currentRef.get()));
        }

        return std::make_tuple(
            currentNode,
            StrView{
                begin(expandingBranch) + advanceOnCanonize,
                expansionIt + 1});
    }

private:
    template< typename StrView >
    constexpr StrView& advanceStringView(StrView& view) const noexcept
    {
        using std::begin;
        using std::end;

        using std::empty;

        if (!empty(view)) {
            view = {begin(view) + 1, end(view)};
        }

        return view;
    }

    template< typename Node >
    constexpr auto canonizeLink(
        Node& root,
        Node& node,
        typename Node::StringViewType wordPath,
        bool& advanceOnCanonize) const
    {
        using std::empty;

        using StrView = typename Node::StringViewType;
        auto link = node.suffixLink();

        advanceOnCanonize = !link && empty(wordPath);
        auto result = (link) ? Canonizer((*link).get(), wordPath)
                             : Canonizer(root, advanceStringView(wordPath));

        return std::make_tuple(
            std::ref(const_cast< Node& >(std::get< 0 >(result))),
            std::get< 1 >(result));
    }
};

} // namespace CPO

inline namespace Cust {

template< auto Canonizer = canonize, auto TestAndSplitter = testAndSplit<> >
inline constexpr CPO::Update< Canonizer, TestAndSplitter > update{};

} // namespace Cust

} // namespace Ukkonen
} // namespace Builders
} // namespace SuffixTree
} // namespace Penjing
