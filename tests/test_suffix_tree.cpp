
#include "gtest/gtest.h"
#include "suffix_tree/suffix_tree.hpp"

struct SuffixTreeTest : testing::Test {
};

TEST_F(SuffixTreeTest, CreateSTree) {
    st::suffix_tree<> stree('$');
    stree.emplace("cacao$");
    auto cao_substr = stree.is_substring("cao$");
    ASSERT_TRUE(cao_substr);
}
