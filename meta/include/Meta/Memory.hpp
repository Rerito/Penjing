// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <memory>
#include <type_traits>

namespace Meta {

/*!
 * \brief A custom deleter destroying and freeing memory given a custom
 *        allocator.
 */
template< typename Allocator, typename DoDestroy = std::true_type >
class CustomAllocatorDeleter
{
public:
    using AllocatorType = Allocator;
    using AllocatorTraits = std::allocator_traits< AllocatorType >;

    constexpr CustomAllocatorDeleter(AllocatorType alloc) noexcept(
        noexcept(AllocatorType(std::move(alloc))))
        : _alloc(std::move(alloc))
    {}

    template< typename T >
    constexpr void operator()(T* pointer)
    {
        if constexpr (DoDestroy::value) {
            AllocatorTraits::destroy(_alloc, pointer);
        }

        AllocatorTraits::deallocate(_alloc, pointer, 1u);
    }

private:
    AllocatorType _alloc;
};

/*!
 * \brief Convenience alias to a unique_ptr using a custom allocator for object
 *        destruction and deletion.
 */
template< typename T, typename Allocator >
using CustomAllocatorUPtr =
    std::unique_ptr< T, CustomAllocatorDeleter< Allocator > >;

/*!
 * \brief Equivalent to std::make_unique for CustomAllocatorUPtr objects.
 *
 * \param alloc The custom allocator used to allocate, construct, destruct and
 *        deallocate the object.
 * \param args... Arguments to forward for the construction of the object of
 *        type T.
 *
 * \return A CustomAllocatorUPtr owning the allocated object.
 */
template< typename T, typename Allocator, typename... Args >
auto allocateUnique(Allocator&& alloc, Args&&... args)
{
    using AllocatorType = std::decay_t< Allocator >;
    using AllocatorTraits = std::allocator_traits< AllocatorType >;
    using NoDestroyPtr = std::unique_ptr<
        T,
        CustomAllocatorDeleter< AllocatorType, std::false_type > >;

    auto storage = NoDestroyPtr(
        AllocatorTraits::allocate(alloc, 1u),
        typename NoDestroyPtr::deleter_type(alloc));

    AllocatorTraits::construct(
        alloc,
        storage.get(),
        std::forward< Args >(args)...);

    return CustomAllocatorUPtr< T, AllocatorType >{
        storage.release(),
        CustomAllocatorDeleter< AllocatorType >(
            std::forward< Allocator >(alloc))};
}

} // namespace Meta
