// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Builders/Ukkonen/Canonize.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Split.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/TestAndSplit.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Update.hpp>

#include <BananaFixture.hpp>
#include <NodeFactory.hpp>

using namespace Penjing::SuffixTree::Builders::Ukkonen;
using namespace Penjing::SuffixTree::Core;
using namespace Penjing::SuffixTree::Test;

using UkkonenUpdateFixture = BananaFixture;

TEST_F(UkkonenUpdateFixture, Basic)
{
    NodeFactory< NodeType > factory;
    std::string can_ = "can$";
    auto result = update<>(_nodes.at(0), _nodes.at(0), can_, 0, factory);

    // We added the expansion for the first character in the new string "can$"
    // We should end up in root
    ASSERT_EQ(&std::get< 0 >(result), &_nodes.at(0));

    auto cTransition = _nodes.at(0).transition('c');
    ASSERT_TRUE(!!cTransition);
}
