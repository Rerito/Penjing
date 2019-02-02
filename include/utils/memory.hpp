
#pragma once

#include <memory>
#include "utils/forward_macro.hpp"

namespace memory {


struct no_destruction {
    template <typename Alloc, typename T>
    void destroy(Alloc&, T*) const noexcept {}
};

struct with_destruction {
    template <typename Alloc, typename T>
    void destroy(Alloc& alloc, T* p) const noexcept {
        std::allocator_traits<Alloc>::destroy(alloc, p);
    }
};

template <typename Alloc, typename DestroyPolicy = with_destruction>
class custom_alloc_deleter : DestroyPolicy {
public:
    using allocator_type = Alloc;
    using allocator_traits = std::allocator_traits<allocator_type>;

    custom_alloc_deleter() noexcept(noexcept(allocator_type())) : alloc_() {}
    explicit custom_alloc_deleter(allocator_type const& alloc) noexcept : alloc_(alloc) {}

    template <typename T>
    void operator()(T *p) {
        DestroyPolicy::destroy(alloc_, p);
        allocator_traits::deallocate(alloc_, p, 1);
    }
private:
    allocator_type alloc_;
};

template <typename T, typename Alloc>
using custom_alloc_unique_ptr = std::unique_ptr<T, custom_alloc_deleter<Alloc, with_destruction> >;

template <typename T, typename Alloc, typename... Args>
auto allocate_unique(Alloc&& alloc, Args&&... args) {
    using alloc_type = std::remove_reference_t<Alloc>;
    using alloc_traits = std::allocator_traits<alloc_type>;
    auto ptrStorage = std::unique_ptr<T, custom_alloc_deleter<alloc_type, no_destruction> > (
        alloc_traits::allocate(alloc, 1),
        custom_alloc_deleter<alloc_type, no_destruction>(alloc)
    );
    alloc_traits::construct(alloc, ptrStorage.get(), CPPFWD(args)...);
    return std::unique_ptr<T, custom_alloc_deleter<alloc_type, with_destruction> >(
        ptrStorage.release(),
        custom_alloc_deleter<alloc_type, with_destruction>(CPPFWD(alloc))
    );
}


} // namespace memory
