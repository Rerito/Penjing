// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Builders/Ukkonen/Split.hpp>

#include <BananaFixture.hpp>
#include <NodeFactory.hpp>

using namespace std::string_literals;

using namespace Penjing::SuffixTree;
using namespace Penjing::SuffixTree::Test;

using UkkonenSplitFixture = BananaFixture;

TEST_F(UkkonenSplitFixture, InvalidBranchingPoint)
{
    NodeFactory< NodeType > factory;
    auto& origin = _nodes.at(0);
    auto t = mutableTransition(origin, 'b');

    ASSERT_TRUE(!!t);
    ASSERT_DEBUG_DEATH(Builders::Ukkonen::split(origin, *t, 0, factory), "");
    ASSERT_DEBUG_DEATH(
        Builders::Ukkonen::split(
            origin,
            *t,
            std::ranges::size(_banana),
            factory),
        "");
}

TEST_F(UkkonenSplitFixture, CommonSplit)
{
    NodeFactory< NodeType > factory;

    auto& origin = _nodes.at(0);
    auto t_banana_ = mutableTransition(origin, 'b');
    auto const& label = (*t_banana_).get().label();
    auto* originalTarget = (*t_banana_).get().target();

    auto ban = "ban"s;

    ASSERT_TRUE(!!t_banana_);
    ASSERT_EQ(label, _banana);

    auto& newNode = Builders::Ukkonen::split(origin, *t_banana_, 3, factory);

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
