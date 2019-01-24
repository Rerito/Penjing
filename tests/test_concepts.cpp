

#include "gtest/gtest.h"
#include "meta/concepts.hpp"
#include "meta/has_size.hpp"
#include "meta/is_string_view.hpp"

#include <string_view>
#include <vector>

using namespace meta;

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
