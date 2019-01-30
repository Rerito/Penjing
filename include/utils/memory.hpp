
#pragma once

#include <memory>
#include "utils/forward_macro.hpp"

namespace memory {

struct no_destruction {
    template <typename T>
    void destroy(T*) const noexcept {}
};

struct with_destruction {
    template <typename T>
    void destroy(T* p) const noexcept {
        std::destroy(p, p+1);
    }
};

template <typename Alloc, typename DestroyPolicy = with_destruction>
struct custom_alloc_deleter : DestroyPolicy {
    template <typename T>
    void operator()(T *p) const {
        DestroyPolicy::destroy(p);
        Alloc{}.deallocate(p, 1);
    }
};

template <typename T, typename Alloc>
using custom_alloc_unique_ptr = std::unique_ptr<T, custom_alloc_deleter<Alloc, with_destruction> >;

template <typename T, typename Alloc, typename... Args>
auto make_unique(Args&&... args) {
    auto ptrStorage = std::unique_ptr<T, custom_alloc_deleter<Alloc, no_destruction> > (Alloc{}.allocate(1));
    new (ptrStorage.get()) T(CPPDWD(args)...);
    return std::unique_ptr<T, custom_alloc_deleter<Alloc, with_destruction> >(ptrStorage.release());
}


} // namespace memory
