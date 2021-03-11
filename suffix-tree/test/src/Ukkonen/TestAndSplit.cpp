// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <SuffixTree/Builders/Ukkonen/Split.hpp>
#include <SuffixTree/Builders/Ukkonen/TestAndSplit.hpp>

#include <BananaFixture.hpp>
#include <NodeFactory.hpp>

using namespace std::string_literals;
using namespace SuffixTree;
using namespace SuffixTree::Builders::Ukkonen;

using UkkonenTestAndSplitFixture = SuffixTree::Test::BananaFixture;

TEST_F(UkkonenTestAndSplitFixture, Common)
{
    SuffixTree::Test::NodeFactory< NodeType > factory;

    auto& origin = _nodes.at(0);

    std::string ban = "ban";

    auto const& t = origin.transition('b', Core::UnsafeTag{});
    auto const originalTarget = t.target();

    auto [isEndPoint, newNode] = testAndSplit< SplitPolicy >(
        origin,
        std::string_view{ban},
        't',
        factory);

    ASSERT_FALSE(isEndPoint);
    ASSERT_EQ(ban, t.label());
    ASSERT_EQ(&newNode, t.target());

    auto const& t2 = newNode.transition('a', Core::UnsafeTag{});
    ASSERT_EQ(originalTarget, t2.target());
}

TEST_F(UkkonenTestAndSplitFixture, EndPoint)
{
    SuffixTree::Test::NodeFactory< NodeType > factory;

    auto& origin = _nodes.at(0);
    std::string ban = "ban";

    auto [isEndPoint, newNode] = testAndSplit< SplitPolicy >(
        origin,
        std::string_view{ban},
        'a',
        factory);

    ASSERT_TRUE(isEndPoint);
    ASSERT_EQ(&origin, &newNode);
}
