// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <SuffixTree/Builders/Ukkonen/Split.hpp>

#include <BananaFixture.hpp>
#include <NodeFactory.hpp>

using namespace std::string_literals;

using UkkonenSplitFixture = SuffixTree::Test::BananaFixture;

TEST_F(UkkonenSplitFixture, InvalidBranchingPoint)
{
    SuffixTree::Test::NodeFactory< NodeType > factory;
    auto& origin = _nodes.at(0);
    auto t = mutableTransition(origin, 'b');

    ASSERT_TRUE(!!t);
    ASSERT_DEATH(
        SuffixTree::Builders::Ukkonen::split(origin, *t, 0, factory),
        "");
    ASSERT_DEATH(
        SuffixTree::Builders::Ukkonen::split(
            origin,
            *t,
            std::ranges::size(_banana),
            factory),
        "");
}

TEST_F(UkkonenSplitFixture, CommonSplit)
{
    SuffixTree::Test::NodeFactory< NodeType > factory;

    auto& origin = _nodes.at(0);
    auto t_banana_ = mutableTransition(origin, 'b');
    auto const& label = (*t_banana_).get().label();
    auto* originalTarget = (*t_banana_).get().target();

    auto ban = "ban"s;

    ASSERT_TRUE(!!t_banana_);
    ASSERT_EQ(label, _banana);

    auto& newNode =
        SuffixTree::Builders::Ukkonen::split(origin, *t_banana_, 3, factory);

    ASSERT_EQ(ban, label);
    ASSERT_EQ(&newNode, (*t_banana_).get().target());

    auto t_ana_ = newNode['a'];
    auto ana_ = "ana$"s;

    ASSERT_TRUE(!!t_ana_);
    auto const& newLabel = (*t_ana_).get().label();
    auto* newTarget = (*t_ana_).get().target();

    ASSERT_EQ(newLabel, ana_);
    ASSERT_EQ(newTarget, originalTarget);
}
