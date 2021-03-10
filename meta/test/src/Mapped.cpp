// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <map>
#include <unordered_map>
#include <vector>

#include <gtest/gtest.h>

#include <Meta/CustomizationPoints/Mapped.hpp>

TEST(CustomizationPoints, mappedAt)
{
    std::unordered_map< int, int > m{{1, 2}, {3, 4}, {5, 6}};

    auto at1 = Meta::mappedAt(m, 1);
    ASSERT_TRUE(!!at1);
    ASSERT_EQ(2, *at1);

    auto at2 = Meta::mappedAt(m, 2);
    ASSERT_TRUE(!at2);
}

TEST(CustomizationPoints, unsafeMappedAt)
{
    std::unordered_map< int, int > m{{1, 2}, {3, 4}, {5, 6}};

    int& at1 = Meta::unsafeMappedAt(m, 1);
    ASSERT_EQ(at1, 2);

    ASSERT_THROW(Meta::unsafeMappedAt(m, 2), std::invalid_argument);
}

TEST(CustomizationPoints, mapped)
{
    std::map< int, int > m{{1, 2}, {3, 4}, {5, 6}};

    std::vector< int > values{2, 4, 6};
    std::vector< int > result;

    std::ranges::copy(Meta::mapped(m), std::back_inserter(result));

    ASSERT_EQ(values, result);
}
