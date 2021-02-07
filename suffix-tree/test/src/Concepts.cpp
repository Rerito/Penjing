// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#include <string>
#include <string_view>

#include "gtest/gtest.h"

#include <SuffixTree/Concepts/String.hpp>
#include <SuffixTree/Concepts/StringView.hpp>

TEST(ConceptString, BasicString)
{
    using SuffixTree::Concepts::String;
    static_assert(
        String< std::string >,
        "std::string should satisfy the String concept");

    static_assert(
        String< std::basic_string< int > >,
        "A std::basic_string specialization should satisfy the String concept");
}

TEST(ConceptStringView, BasicStringView)
{
    using SuffixTree::Concepts::StringView;
    static_assert(
        StringView< std::string, std::string_view >,
        "std::string and std::string_view should satisfy "
        "the StringView concept");

    static_assert(
        StringView< std::basic_string< int >, std::basic_string_view< int > >,
        "A std::basic_string and the corresponding std::basic_string_view "
        "specializations should satisfy the StringView concept");
}
