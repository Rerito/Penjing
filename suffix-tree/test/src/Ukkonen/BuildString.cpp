// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Algorithm/CompareNodes.hpp>

#include <Penjing/SuffixTree/Builders/Ukkonen/BuildString.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Canonize.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Split.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/TestAndSplit.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Update.hpp>

#include <BananaFixture.hpp>
#include <Dump.hpp>
#include <NodeFactory.hpp>

using namespace Penjing::SuffixTree;
using namespace Penjing::SuffixTree::Test;
using namespace Penjing::SuffixTree::Builders::Ukkonen;

using UkkonenBuildStringFixture = BananaFixture;

inline constexpr CPO::BuildString<
    CanonizePolicy,
    UpdatePolicy< CanonizePolicy, TestAndSplitPolicy< SplitPolicy > > >
    buildString{};

TEST_F(UkkonenBuildStringFixture, Mississipi)
{
    NodeFactory< NodeType > factory{};
    auto& root = factory();
    std::string mississipi = "mississipi$";
    ::buildString(root, mississipi, factory);
    dump(root, std::cout);
}

TEST_F(UkkonenBuildStringFixture, Build)
{
    NodeFactory< NodeType > factory{};

    auto& root = factory();
    ::buildString(root, _banana, factory);
    EXPECT_TRUE(Algorithm::compareNodes(root, _nodes.at(0)));

    std::string cacao = "cacao$";
    ::buildString(root, cacao, factory);

    std::string curacao = "curacao$";
    ::buildString(root, curacao, factory);

    std::string burundi = "burundi$";
    ::buildString(root, burundi, factory);
    dump(root, std::cout);
}

TEST_F(UkkonenBuildStringFixture, mississippixsissy)
{
    NodeFactory< NodeType > factory{};

    auto& root = factory();
    std::string test = "mississippixsissy$";
    ::buildString(root, test, factory);
    dump(root, std::cout);
}
