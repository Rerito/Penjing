// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <SuffixTree/Algorithm/CompareNodes.hpp>

#include <BananaFixture.hpp>

using CompareNodesFixture = SuffixTree::Test::BananaFixture;

TEST_F(CompareNodesFixture, Compare)
{
    ASSERT_TRUE(
        SuffixTree::Algorithm::compareNodes(_nodes.at(0), _nodes.at(0)));
    ASSERT_FALSE(
        SuffixTree::Algorithm::compareNodes(_nodes.at(0), _nodes.at(1)));
}
