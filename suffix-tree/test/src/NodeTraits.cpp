// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <string>

#include "gtest/gtest.h"

#include <Penjing/SuffixTree/Core/StdUnorderedMapNodeTraits.hpp>

TEST(NodeTraits, StdUnorderedMap)
{

    using Traits = Penjing::SuffixTree::Core::StdUnorderedMapNodeTraits<
        std::allocator< int > >;

    static_assert(
        std::is_same_v<
            std::unordered_map< int, std::string >,
            Traits::StorageType< int, std::string > >,
        "StdUnorderedMapNodeTraits with an std::allocator should give "
        "the default unordered_map");
}
