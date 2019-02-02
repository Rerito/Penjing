
#include "gtest/gtest.h"
#include "utils/memory.hpp"

struct life_tracker {
    static size_t nb_construct;
    static size_t nb_destruct;

    life_tracker() { ++nb_construct; }
    ~life_tracker() { ++nb_destruct; }
};

size_t life_tracker::nb_construct = 0u;
size_t life_tracker::nb_destruct = 0u;

template <typename T>
struct test_allocator : std::allocator<T> {
    static size_t nb_alloc;
    static size_t nb_dealloc;

    void deallocate(T *p, size_t n) {
        nb_dealloc += n;
        std::allocator<T>::deallocate(p, n);
    }

    T* allocate(size_t n) {
        auto ptr = std::allocator<T>::allocate(n);
        nb_alloc += n;
        return ptr;
    }
        
};

template <typename T>
size_t test_allocator<T>::nb_alloc = 0;

template <typename T>
size_t test_allocator<T>::nb_dealloc = 0;

struct MemoryTest : testing::Test {
};

TEST_F(MemoryTest, TestMakeUnique) {
    test_allocator<life_tracker> a{};
    using memory::allocate_unique;
    {
        auto ptr = allocate_unique<life_tracker>(a);
    }
    ASSERT_EQ(test_allocator<life_tracker>::nb_alloc, test_allocator<life_tracker>::nb_dealloc);
    ASSERT_EQ(test_allocator<life_tracker>::nb_alloc, 1u);
    ASSERT_EQ(life_tracker::nb_construct, life_tracker::nb_destruct);
    ASSERT_EQ(life_tracker::nb_construct, 1u);
}
