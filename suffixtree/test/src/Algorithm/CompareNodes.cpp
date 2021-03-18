// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Algorithm/CompareNodes.hpp>

#include <BananaFixture.hpp>

using namespace Penjing::SuffixTree;
using namespace Penjing::SuffixTree::Test;

using CompareNodesFixture = BananaFixture;

TEST_F(CompareNodesFixture, Compare)
{
    ASSERT_TRUE(Algorithm::compareNodes(_nodes.at(0), _nodes.at(0)));
    ASSERT_FALSE(Algorithm::compareNodes(_nodes.at(0), _nodes.at(1)));
}
