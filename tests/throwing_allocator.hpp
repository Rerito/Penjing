
#pragma once

#include <memory>

template <typename T, typename Limit>
struct throwing_allocator : std::allocator<T> {
    static size_t nb_alloc;
    static size_t nb_dealloc;

    throwing_allocator() noexcept {}
    throwing_allocator(throwing_allocator const&) noexcept = default;
    template <typename U>
    throwing_allocator(throwing_allocator<U, Limit> const&) noexcept {}
    void deallocate(T *p, size_t n) {
        nb_dealloc += n;
        std::allocator<T>::deallocate(p, n);
    }

    T* allocate(size_t n) {
        if (nb_alloc + n < Limit::value) {
            auto ptr = std::allocator<T>::allocate(n);
            nb_alloc += n;
            return ptr;
        } else {
            throw std::bad_alloc();
        }
    }

    template <typename Other>
    struct rebind {
        using other = throwing_allocator<Other, Limit>;
    };
};

template <typename T, typename Limit>
size_t throwing_allocator<T, Limit>::nb_alloc = 0u;

template <typename T, typename Limit>
size_t throwing_allocator<T, Limit>::nb_dealloc = 0u;
