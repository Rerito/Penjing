

#include "gtest/gtest.h"
#include "concepts.hpp"
#include "has_size.hpp"
#include <vector>

struct ConceptsTest : testing::Test {
};

TEST_F(ConceptsTest, HasSize) {
    using namespace meta;
    ASSERT_TRUE(models<adl_concepts::has_size(std::vector<int>)>::value);
    ASSERT_FALSE(models<adl_concepts::has_size(int)>::value);
}
