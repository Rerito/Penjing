// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <cassert>
#include <memory>
#include <ranges>
#include <type_traits>

#include <Penjing/Meta/IsDetected.hpp>
#include <Penjing/Meta/SelfVariant.hpp>
#include <Penjing/Meta/VariantHelpers.hpp>

namespace Penjing {
namespace Storage {

namespace Traits {

// Convenience alias for the given `StorageTraits` `StorageType` template alias.
template< typename StorageTraits, typename Value >
using StorageType = typename StorageTraits::template StorageType< Value >;

template< typename StorageTraits, typename Value >
using ValueRemovalType = decltype(StorageTraits::remove(
    std::declval<
        std::add_lvalue_reference_t< StorageType< StorageTraits, Value > > >(),
    std::declval< std::add_lvalue_reference_t< Value > >()));

template< typename StorageTraits, typename Storage, typename... Args >
using ValueEmplaceType = decltype((StorageTraits::emplace(
    std::declval< std::add_lvalue_reference_t< Storage > >(),
    std::declval< Args >()...)));

template< typename StorageTraits, typename Value >
using HasValueRemoval =
    Meta::IsDetected< ValueRemovalType, StorageTraits, Value >;

template< typename Value, bool HasNativeRemoval >
struct Element
{
    using Type = Value;
};

template< typename Value >
    requires Meta::NoStorageOffsetVariant<
        Meta::SelfVariant< std::monostate, Value* > >
struct Element< Value, false >
{
    using Type = Meta::SelfVariant< std::monostate, Value >;
};

} // namespace Traits

// Companion class that handles the storage values.
// The template expects two type parameters:
//  - Value The Value type to store.
//  - StorageTraits The traits class defining the storage behavior.
// StorageTraits must have the inner member template alias StorageType and the
// member function `emplace`.
// Additionally, if the `remove` function is defined and is callable with a
// `Value` argument, it will be used as the value removal policy.
//
// The Store will automatically instantiate the proper container and handle
// the removal using a free list policy if the underlying storage does not
// support native removal.
template< typename Value, typename StorageTraits >
class Store
{
private:
    static constexpr bool _hasNativeRemoval()
    {
        return Traits::HasValueRemoval< StorageTraits, Value >::value;
    }

public:
    // If the StorageTraits does not define a value removal policy,
    // we fallback on a free-list policy to try to maximize cache spatial
    // locality.
    using Element =
        typename Traits::Element< Value, _hasNativeRemoval() >::Type;

    using Container = typename StorageTraits::template StorageType< Element >;

private:
    Container _storage;
    Element* _free;
    size_t _activeCount;
    size_t _freeCount;

private:
    static constexpr bool _hasNoExceptRemoval()
    {
        if constexpr (_hasNativeRemoval()) {
            return noexcept(StorageTraits::remove(
                std::declval< Container& >(),
                std::declval< Value&& >()));
        } else {
            return std::is_nothrow_assignable_v< Element, Element* >;
        }
    }

    template< typename... Args >
    static constexpr bool _canEmplace()
    {
        return std::is_convertible_v<
            Traits::ValueEmplaceType< StorageTraits, Container, Args... >,
            std::add_lvalue_reference_t< Element > >;
    }

    template< typename... Args >
    static constexpr bool _hasNoExceptEmplace()
    {
        return noexcept(StorageTraits::emplace(
            std::declval< Container& >(),
            std::declval< Args >()...));
    }

public:
    template< typename... Args >
        requires(_canEmplace< Args... >() && _hasNativeRemoval())
    Value& emplace(Args&&... args) noexcept(_hasNoExceptEmplace< Args&&... >())
    {
        auto& value =
            StorageTraits::emplace(_storage, std::forward< Args >(args)...);
        ++_activeCount;

        return value;
    }

    template< typename... Args >
        requires(_canEmplace< Value&& >())
    Value& emplace(Args&&... args) noexcept(_hasNoExceptEmplace< Value&& >())
    {
        Element* valueVariant = nullptr;
        if (nullptr != _free) {
            valueVariant = _free;
            auto value = Value{std::forward< Args >(args)...};

            // Cannot throw as the _free pointer points to a recycled value.
            _free = std::get< Element* >(*valueVariant);

            *valueVariant = std::move(value);
            --_freeCount;
        } else {
            valueVariant = std::addressof(StorageTraits::emplace(
                _storage,
                Value{std::forward< Args >(args)...}));
        }

        ++_activeCount;

        assert(nullptr != valueVariant);
        // This cannot throw as the variant is currently storing the value.
        return std::get< Value >(*valueVariant);
    }

    void remove(Value&& value) noexcept(_hasNoExceptRemoval()) requires(
        _hasNativeRemoval())
    {
        // The move is used to signal the caller that the object should not be
        // used after the remove.
        StorageTraits::remove(_storage, std::move(value));
        --_activeCount;
    }

    void remove(Value&& value) noexcept(_hasNoExceptRemoval())
    {
        // 1. From the value's reference, deduce the containing variant.
        auto& asVariant = *static_cast< Element* >(
            static_cast< void* >(std::addressof(value)));

        // 2. Destroy the value object and update the freelist.
        asVariant = _free;
        _free = std::addressof(asVariant);
        ++_freeCount;
        --_activeCount;
    }
};

} // namespace Storage
} // namespace Penjing
