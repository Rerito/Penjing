// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#include <string>

#include <gtest/gtest.h>

#include <Penjing/Storage/Bindings/ArrayList.hpp>

#include <Penjing/SuffixTree/Core/NakedTree.hpp>
#include <Penjing/SuffixTree/Core/Tree.hpp>

#include <Penjing/SuffixTree/Builders/TreeBuild.hpp>
#include <Penjing/SuffixTree/Builders/Ukkonen.hpp>

#include <Dump.hpp>

using namespace Penjing;
using namespace Penjing::SuffixTree;
using namespace Penjing::SuffixTree::Test;

TEST(CoreTree, Creation)
{
    using NakedTreeType = Core::NakedTree< std::string, std::string_view >;
    using TreeType = Core::Tree< NakedTreeType >;

    using NodeType = TreeType::NodeType;
    using TransitionType = NodeType::TransitionType;

    static_assert(
        std::is_same_v< TreeType::CharType, char >,
        "The CharType should be defined and equal to char");

    static_assert(
        std::is_same_v<
            NodeType::TransitionContainerType,
            std::unordered_map< char, TransitionType > >,
        "The Node's storage should be an std::unordered_map");

    TreeType tree{};

    ukkonenBuild(tree, std::string{"cacao$"});

    dump(tree.root(), std::cout);
}
