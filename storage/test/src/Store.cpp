// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/Storage/Bindings/ArrayList.hpp>
#include <Penjing/Storage/Store.hpp>

using namespace Penjing::Storage;

using StorageTraits = Bindings::ArrayList< std::allocator< int >, 16u >;
using StoreType = Store< int, StorageTraits >;

TEST(Store, StaticChecks)
{
    static_assert(
        std::is_same_v<
            StoreType::Element,
            Penjing::Meta::SelfVariant< std::monostate, int > >,
        "Element should be a SelfVariant to allow free-list recycling");
}

TEST(Store, Emplace)
{
    StoreType store{};
    auto& element = store.emplace(12);
    ASSERT_EQ(12, element);
}

TEST(Store, EmplaceRemoveEmplace)
{
    StoreType store{};
    auto* element = &store.emplace(12);
    ASSERT_EQ(12, *element);

    store.remove(std::move(*element));
    auto* element2 = &store.emplace(20);
    ASSERT_EQ(20, *element2);
    ASSERT_EQ(element, element2);
}
