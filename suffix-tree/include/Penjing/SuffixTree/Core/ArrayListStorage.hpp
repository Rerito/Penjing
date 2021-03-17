// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <array>
#include <list>
#include <memory>
#include <optional>
#include <ranges>
#include <type_traits>
#include <variant>

namespace Penjing {
namespace SuffixTree {
namespace Core {

namespace Traits {

template< typename... Args >
std::true_type isMonoStateVariant(std::variant< std::monostate, Args... >&&);

template< typename T >
std::false_type isMonoStateVariant(T&&);

template< typename T >
using IsMonoStateVariant = decltype(isMonoStateVariant(
    std::declval< std::remove_cv_t< std::remove_reference_t< T > > >()));

} // namespace Traits

template< typename Value, typename Alloc, size_t ArraySize >
class ArrayListStorage
{
private:
    static constexpr bool _useOptional()
    {
        return !std::is_arithmetic_v< Value > && !std::is_pointer_v< Value > &&
               !Traits::IsMonoStateVariant< Value >::value &&
               !std::is_nothrow_constructible_v< Value >;
    }

    using ValueType =
        std::conditional_t< _useOptional(), std::optional< Value >, Value >;

public:
    using ArrayType = std::array< ValueType, ArraySize >;
    using ArrayAlloc =
        std::allocator_traits< Alloc >::template rebind_alloc< ArrayType >;
    using StorageType = std::list< ArrayType, ArrayAlloc >;

private:
    StorageType _storage;
    size_t _freeItem;

private:
    template< typename... Args >
    static constexpr bool _hasNoExceptEmplace()
    {
        return std::is_nothrow_constructible_v< Value, Args... > &&
               std::is_nothrow_move_assignable_v< Value >;
    }

public:
    template< typename... Args >
    Value& emplace(Args&&... args) noexcept(_hasNoExceptEmplace< Args&&... >())
    {
        if constexpr (_useOptional()) {
            return *doEmplace(std::forward< Args >(args)...);
        } else {
            return doEmplace(std::forward< Args >(args)...);
        }
    }

private:
    template< typename... Args >
    ValueType& doEmplace(Args&&... args) noexcept(
        _hasNoExceptEmplace< Args&&... >())
    {
        if (_storage.empty() || ArraySize == _freeItem) {
            _freeItem = 0;
            _storage.emplace_back();
        }

        _storage.back()[_freeItem] = Value(std::forward< Args >(args)...);

        return _storage.back()[_freeItem++];
    }
};

template< typename Alloc, size_t ArraySize >
struct ArrayListStorageTraits
{
    using AllocatorType = Alloc;

    template< typename Value >
    using StorageType = ArrayListStorage< Value, Alloc, ArraySize >;

    template< typename Value, typename... Args >
    static Value& emplace(
        ArrayListStorage< Value, Alloc, ArraySize >& storage,
        Args&&... args) noexcept(noexcept(storage
                                              .emplace(
                                                  std::declval< Args&& >()...)))
    {
        return storage.emplace(std::forward< Args >(args)...);
    }
};

} // namespace Core
} // namespace SuffixTree
} // namespace Penjing
