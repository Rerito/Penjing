
#include "gtest/gtest.h"
#include "suffix_tree/suffix_tree.hpp"

using stree_t = st::suffix_tree<>;

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
    ASSERT_TRUE(stree.is_substring("cac"));
    ASSERT_FALSE(stree.is_substring("sipp"));
}

TEST_F(SuffixTreeTest, Equality) {
    stree_t t1('$');
    stree_t t2('$');
    stree_t t3('"');
    ASSERT_NE(t1,t3);
    t1.emplace("cocoa$");
    t2.emplace("cocoa$");
    ASSERT_EQ(t1,t2);
    t1.emplace("chocolate$");
    ASSERT_NE(t1,t2);
    t2.emplace("chocolate$");
    ASSERT_EQ(t1,t2);
    t2.emplace("late$");
    ASSERT_TRUE(t1.structurally_equal_to(t2));
}
