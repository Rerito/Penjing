

#include "gtest/gtest.h"
#include "meta/concepts.hpp"
#include "meta/has_size.hpp"
#include <vector>

using namespace meta;

struct ConceptsTest : testing::Test {
};

TEST_F(ConceptsTest, HasSize) {
    ASSERT_TRUE(models<adl_concepts::has_size(std::vector<int>)>::value);
    ASSERT_FALSE(models<adl_concepts::has_size(int)>::value);
}
