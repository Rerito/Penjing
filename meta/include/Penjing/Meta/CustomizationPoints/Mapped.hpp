// Copyright (c) 2021-2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <optional>
#include <type_traits>

#include "../IsDetected.hpp"
#include "../IteratorType.hpp"
#include "../ToReferenceWrapper.hpp"
#include "../ValueType.hpp"

namespace Penjing {
namespace Meta {

namespace Details {

template< typename T >
using SecondType = decltype(std::declval< T >().second);

template< typename T >
concept HasSecond = Detected< SecondType, T >;

template< typename M, typename K >
using FindReturnType = decltype(std::declval< M >().find(std::declval< K >()));

template< typename M, typename K >
concept HasFind = Detected< FindReturnType, M, K >;

struct Throw
{
    template< typename T >
        requires std::is_reference_v< T >
    using MappedType = T;

    template< typename T >
    [[noreturn]] constexpr MappedType< T > failedAccess() const
    {
        throw std::invalid_argument("bad access");
    }
};

struct EmptyOptional
{
    template< typename T >
        requires std::is_reference_v< T >
    using MappedType = std::optional< ToReferenceWrapper< T > >;

    template< typename T >
    constexpr MappedType< T > failedAccess() const noexcept
    {
        return {};
    }
};

} // namespace Details

namespace CustomizationPoints {

/*!
 * \brief Customization point object to access the requested mapped item.
 */
template< typename OnFailedAccess >
struct MappedAt : private OnFailedAccess
{
    /*!
     * \brief Fetch the mapped element of the map-like range for the given key
     *
     * \details The range must fulfill the borrowed_range requirement and must
     *          have a `find` member-function callable with an object of type
     *          Key. The range's value type must have an accessible `second`
     *          data member.
     *
     * \param range The range from which to fetch the item
     * \param key The key of the item to fetch
     *
     * \return An optional containing a reference_wrapper to the desired mapped
     *         element or an empty optional if there is no such element.
     */
    template< typename Range, typename Key >
        requires Details::HasFind< Range&, Key&& > &&
            Details::HasSecond< ValueType< Range > >
    constexpr auto operator()(Range& range, Key&& key) const noexcept(
        noexcept(std::forward< Range >(range).find(std::forward< Key >(key))))
        -> typename OnFailedAccess::template MappedType< decltype((
            std::declval< std::iter_reference_t< IteratorType< Range& > > >()
                .second)) >
    {
        using std::end;

        using ElementRef = decltype((
            std::declval< std::iter_reference_t< IteratorType< Range& > > >()
                .second));
        auto it = range.find(std::forward< Key >(key));

        if (end(range) == it) {
            return OnFailedAccess::template failedAccess< ElementRef >();
        }

        return it->second;
    }
};

} // namespace CustomizationPoints

inline namespace Cust {

inline constexpr CustomizationPoints::MappedAt< Details::EmptyOptional >
    mappedAt{};
inline constexpr CustomizationPoints::MappedAt< Details::Throw >
    unsafeMappedAt{};

} // inline namespace Cust

} // namespace Meta
} // namespace Penjing
