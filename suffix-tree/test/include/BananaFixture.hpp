// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <array>
#include <memory>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <SuffixTree/Algorithm/MutatingNodeAlgorithm.hpp>
#include <SuffixTree/Core/Node.hpp>
#include <SuffixTree/Core/StdUnorderedMapNodeTraits.hpp>

namespace SuffixTree {
namespace Test {

class BananaFixture
    : public ::testing::Test
    , protected Algorithm::MutatingNodeAlgorithm

{
protected:
    using NodeType = Core::Node<
        std::string,
        std::string_view,
        Core::StdUnorderedMapNodeTraits< std::allocator< std::string > > >;

    BananaFixture();
    ~BananaFixture() = default;

protected:
    std::string _banana;
    std::array< NodeType, 11u > _nodes;

    NodeType& n_root;

    // Children of the root
    NodeType& n_a;
    NodeType& n_banana_;
    NodeType& n_na;
    NodeType& n_;

    // Children of a
    NodeType& n_a_;
    NodeType& n_ana;

    // Children of na
    NodeType& n_na_;
    NodeType& n_nana_;

    // Children of ana
    NodeType& n_ana_;
    NodeType& n_anana_;
};

} // namespace Test
} // namespace SuffixTree
