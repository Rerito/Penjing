// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#include <sstream>

#include <gtest/gtest.h>

#include <Penjing/Storage/Bindings/StdMap.hpp>

#include <Penjing/SuffixTree/Algorithm/CompareNodes.hpp>

#include <Penjing/SuffixTree/Builders/Ukkonen/Build.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Canonize.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Split.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/TestAndSplit.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen/Update.hpp>

#include <Penjing/SuffixTree/Core/Node.hpp>

#include <Dump.hpp>
#include <NodeFactory.hpp>
#include <ToStream.hpp>

using namespace Penjing::SuffixTree;
using namespace Penjing::SuffixTree::Test;
using namespace Penjing::SuffixTree::Builders::Ukkonen;

inline constexpr CPO::Build<
    CanonizePolicy,
    UpdatePolicy< CanonizePolicy, TestAndSplitPolicy< SplitPolicy > > >
    build{};

using NodeType = Core::Node<
    std::string,
    std::string_view,
    Penjing::Storage::Bindings::StdMap< std::allocator< std::string > > >;

// Note on build tests:
// The compact string representation of the tree is the concatenation
// of the transition labels as seen going through a BFS
// Since we are using a regular `std::map` as the transition container we
// also get them in the lexicographical order
//
// For instance the string "cacao$" has the following suffix tree:
// o - [$] - o
//   - [a] - o - [cao$] - o
//             - [o$] - o
//   - [ca] - o - [cao$] - o
//              - [o$] - o
//   - [o$] - o
//
// Which leads to the compact representation:
// [$][a][ca][o$][cao$][o$][cao$][o$]

namespace {

void treeBuildTest(std::string const& str, std::string const& expectedTree)
{
    NodeFactory< NodeType > factory{};
    std::stringstream output;
    auto& root = factory();
    ::build(root, str, factory);
    toStream(root, output);

    ASSERT_EQ(output.str(), expectedTree);
}

}

TEST(UkkonenBuild, Cacao)
{
    std::string cacao = "cacao$";
    std::string expectedTree = "[$][a][ca][o$][cao$][o$][cao$][o$]";

    treeBuildTest(cacao, expectedTree);
}

TEST(UkkonenBuild, Mississippi)
{

    std::string mississipi = "mississippi$";

    std::string expectedTree =
        "[$][i][mississippi$][p][s][$][ppi$][ssi][i$][pi$][i][si][ppi$]"
        "[ssippi$][ppi$][ssippi$][ppi$][ssippi$]";

    treeBuildTest(mississipi, expectedTree);
}

TEST(UkkonenBuild, GST)
{
    NodeFactory< NodeType > factory{};

    std::string banana = "banana$";
    auto& root = factory();
    ::build(root, banana, factory);

    std::string cacao = "cacao$";
    ::build(root, cacao, factory);

    std::string curacao = "curacao$";
    ::build(root, curacao, factory);

    std::string burundi = "burundi$";
    ::build(root, burundi, factory);
    dump(root, std::cout);
}

TEST(UkkonenBuild, Mississippixsissy)
{
    std::string test = "mississippixsissy$";
    std::string expectedTree =
        "[$][i][mississippixsissy$][p][s][xsissy$][y$][ppixsissy$][ss][xsissy$]"
        "[ixsissy$][pixsissy$][i][s][y$][i][y$][ppixsissy$][ss][i][y$]"
        "[ppixsissy$][ssippixsissy$][ippixsissy$][y$][ppixsissy$]"
        "[ssippixsissy$]";

    treeBuildTest(test, expectedTree);
}
