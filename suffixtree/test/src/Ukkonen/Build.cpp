// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Algorithm/CompareNodes.hpp>

#include <Penjing/SuffixTree/Builders/Ukkonen/Build.hpp>
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

using UkkonenBuildFixture = BananaFixture;

inline constexpr CPO::Build<
    CanonizePolicy,
    UpdatePolicy< CanonizePolicy, TestAndSplitPolicy< SplitPolicy > > >
    build{};

TEST_F(UkkonenBuildFixture, Mississipi)
{
    NodeFactory< NodeType > factory{};
    auto& root = factory();
    std::string mississipi = "mississipi$";
    ::build(root, mississipi, factory);
    dump(root, std::cout);
}

TEST_F(UkkonenBuildFixture, Build)
{
    NodeFactory< NodeType > factory{};

    auto& root = factory();
    ::build(root, _banana, factory);
    EXPECT_TRUE(Algorithm::compareNodes(root, _nodes.at(0)));

    std::string cacao = "cacao$";
    ::build(root, cacao, factory);

    std::string curacao = "curacao$";
    ::build(root, curacao, factory);

    std::string burundi = "burundi$";
    ::build(root, burundi, factory);
    dump(root, std::cout);
}

TEST_F(UkkonenBuildFixture, mississippixsissy)
{
    NodeFactory< NodeType > factory{};

    auto& root = factory();
    std::string test = "mississippixsissy$";
    ::build(root, test, factory);
    dump(root, std::cout);
}
