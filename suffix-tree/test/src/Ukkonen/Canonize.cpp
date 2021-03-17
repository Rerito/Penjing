// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <gtest/gtest.h>

#include <Penjing/SuffixTree/Builders/Ukkonen/Canonize.hpp>

#include <BananaFixture.hpp>

using namespace std::string_literals;

using namespace Penjing::SuffixTree;
using namespace Penjing::SuffixTree::Test;

using UkkonenCanonizeFixture = BananaFixture;

TEST_F(UkkonenCanonizeFixture, Explicit)
{
    std::string ana = "ana"s;
    auto [node, path] =
        Builders::Ukkonen::canonize(n_root, std::string_view{ana});

    ASSERT_TRUE(std::ranges::empty(path));
    ASSERT_EQ(&node, &n_ana);
}

TEST_F(UkkonenCanonizeFixture, Implicit)
{
    std::string anan = "anan"s;

    std::string n = "n"s;

    auto [node, path] =
        Builders::Ukkonen::canonize(n_root, std::string_view{anan});

    ASSERT_EQ(path, n);
    ASSERT_EQ(&node, &n_ana);
}

TEST_F(UkkonenCanonizeFixture, Deepest)
{
    std::string anana_ = "anana$"s;

    auto [node, path] =
        Builders::Ukkonen::canonize(n_root, std::string_view{anana_});

    ASSERT_TRUE(std::ranges::empty(path));
    ASSERT_EQ(&node, &n_anana_);
}
