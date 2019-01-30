
#include "gtest/gtest.h"
#include "suffix_tree/suffix_tree.hpp"

struct SuffixTreeTest : testing::Test {
};

TEST_F(SuffixTreeTest, CreateSTree) {
    st::suffix_tree<> stree('$');
    stree.emplace("cacao$");
    auto cao_substr = stree.is_substring("cao$");
    auto kao_substr = stree.is_substring("kao$");  
    ASSERT_TRUE(cao_substr);
    ASSERT_FALSE(kao_substr);
    ASSERT_TRUE(stree.is_substring("cac"));
    stree.emplace("mississipi$");
    ASSERT_TRUE(stree.is_substring("sipi$"));
}
