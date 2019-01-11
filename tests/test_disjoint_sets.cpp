
#include "gtest/gtest.h"
#include "datastruct/disjoint_sets.hpp"

struct DisjointSetsTestCase : testing::Test {
    disjoint_sets<int> sets_;

    void SetUp() override {
        sets_.make_set(1);
        sets_.make_set(2);
        sets_.make_set(3);
        sets_.make_set(4);
    }

    void TearDown() override {
        sets_.clear();
    }
};

TEST_F(DisjointSetsTestCase, UnionByRank) {
    sets_.make_union(1, 2);
    ASSERT_EQ(1, sets_.find(1).rank);
    sets_.make_union(1, 3);
    ASSERT_EQ(1, sets_.find(1).rank);
}

TEST_F(DisjointSetsTestCase, MakeUnion) {
    sets_.make_union(1, 2);
    ASSERT_TRUE(sets_.is_in_same_set(1, 2));
    ASSERT_FALSE(sets_.is_in_same_set(1, 3));
    sets_.make_union(1, 3);
    ASSERT_TRUE(sets_.is_in_same_set(2, 3));
    ASSERT_FALSE(sets_.is_in_same_set(2, 4));
}
