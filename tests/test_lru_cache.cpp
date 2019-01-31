
#include "gtest/gtest.h"
#include "datastruct/lru_cache.hpp"

struct LRUCacheTest : testing::Test {
    
};

TEST_F(LRUCacheTest, SaturatedCache) {
    lru_cache<int, int> cache(3u);
    cache.emplace(12,12);
    cache.emplace(1,1);
    cache.emplace(2,2);
    auto v = cache.get(12);
    ASSERT_EQ(v,12);
    cache.emplace(3,3);
    ASSERT_THROW(cache.get(1), std::runtime_error);
}

TEST_F(LRUCacheTest, Equality) {
    lru_cache<int,int> cache(3u);
    lru_cache<int,int> cache2(3u);

    cache.emplace(1,1);
    cache2.emplace(1,1);
    ASSERT_EQ(cache, cache2);
    cache2.emplace(2,2);
    ASSERT_NE(cache, cache2);
}
