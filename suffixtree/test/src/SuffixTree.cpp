// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <string>

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Core/NakedTree.hpp>

TEST(CoreTree, Creation)
{
    using TreeType =
        Penjing::SuffixTree::Core::NakedTree< std::string, std::string_view >;

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
}
