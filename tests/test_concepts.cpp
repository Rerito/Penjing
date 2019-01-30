

#include "gtest/gtest.h"
#include "meta/concepts.hpp"
#include "meta/has_size.hpp"
#include "meta/is_hash.hpp"
#include "meta/is_string_view.hpp"

#include <string_view>
#include <functional>
#include <vector>

using namespace meta;

struct not_a_hash {
    std::string operator()(std::string const&);
};

struct ConceptsTest : testing::Test {
};

TEST_F(ConceptsTest, HasSize) {
    ASSERT_TRUE(models<adl_concepts::has_size(std::vector<int>)>::value);
    ASSERT_FALSE(models<adl_concepts::has_size(int)>::value);
}

TEST_F(ConceptsTest, IsStringView) {
    auto true_ = models<is_string_view<char*, size_t>(std::string_view)>::value;
    ASSERT_TRUE(true_);
}

TEST_F(ConceptsTest, IsHash) {
    auto true_ = models<is_hash<int>(std::hash<int>)>::value;
    auto false_ = models<is_hash<std::string>(not_a_hash)>::value;
    auto false_2= models<is_hash<int>(not_a_hash)>::value;
    ASSERT_TRUE(true_);
    ASSERT_FALSE(false_);
    ASSERT_FALSE(false_2);
}
