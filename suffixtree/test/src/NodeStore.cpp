// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Core/ArrayListStorage.hpp>
#include <Penjing/SuffixTree/Core/NodeStore.hpp>

using StorageTraits = Penjing::SuffixTree::Core::
    ArrayListStorageTraits< std::allocator< int >, 16u >;
using NodeStore = Penjing::SuffixTree::Core::NodeStore< int, StorageTraits >;

TEST(NodeStore, StaticChecks)
{
    static_assert(
        std::is_same_v<
            NodeStore::NodeElement,
            Penjing::Meta::SelfVariant< std::monostate, int > >,
        "NodeElement should be a SelfVariant to allow free-list recycling");
}

TEST(NodeStore, Emplace)
{
    NodeStore store{};
    auto& node = store.addNode(12);
    ASSERT_EQ(12, node);
}

TEST(NodeStore, EmplaceRemoveEmplace)
{
    NodeStore store{};
    auto* node = &store.addNode(12);
    ASSERT_EQ(12, *node);

    store.removeNode(std::move(*node));
    auto* node2 = &store.addNode(20);
    ASSERT_EQ(20, *node2);
    ASSERT_EQ(node, node2);
}
