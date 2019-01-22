
#include "gtest/gtest.h"
#include "suffix_tree/suffix_tree.hpp"

struct SuffixTreeTest : testing::Test {
};

TEST_F(SuffixTreeTest, CreateSTree) {
    suffix_tree<> st('$');
}
