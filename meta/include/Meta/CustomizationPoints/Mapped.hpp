// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <functional>
#include <optional>
#include <ranges>
#include <type_traits>

#include "../IsDetected.hpp"
#include "../ToReferenceWrapper.hpp"

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
    constexpr MappedType< T > failedAccess() const noexcept { return {}; }
};

} // namespace Details

namespace CustomizationPoints {

/*!
 * \brief Customization point object that offers view on the mapped values of
 *        a map-like range.
 */
struct Mapped
{
    /*!
     * \brief Retrieve a view to the mapped elements of the given range.
     *
     * \details The given range must fulfill the borrowed_range concept and its
     *          value type should have an available `second` data member.
     *
     * \param range The range to view.
     */
    template< typename Range >
        requires std::ranges::borrowed_range< Range& > &&
            Details::HasSecond< std::ranges::range_value_t< Range > >
    constexpr auto operator()(Range& range) const noexcept
    {
        return std::ranges::views::transform(
            range,
            [](auto& mappedElement) noexcept {
                return std::ref(mappedElement.second);
            });
    }
};

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
        requires std::ranges::borrowed_range< Range& > &&
            Details::HasFind< Range&, Key&& > &&
            Details::HasSecond< std::ranges::range_value_t< Range > >
    constexpr auto operator()(Range& range, Key&& key) const noexcept(
        noexcept(std::forward< Range >(range).find(std::forward< Key >(key))))
        -> typename OnFailedAccess::MappedType<
            decltype((std::declval< std::ranges::range_reference_t< Range& > >()
                          .second)) >
    {
        using ElementRef = decltype((
            std::declval< std::ranges::range_reference_t< Range& > >().second));
        auto it = range.find(std::forward< Key >(key));

        if (std::ranges::end(range) == it) {
            return OnFailedAccess::template failedAccess< ElementRef >();
        }

        return it->second;
    }
};

} // namespace CustomizationPoints

inline namespace Cust {

inline constexpr CustomizationPoints::Mapped mapped{};
inline constexpr CustomizationPoints::MappedAt< Details::EmptyOptional >
    mappedAt{};
inline constexpr CustomizationPoints::MappedAt< Details::Throw >
    unsafeMappedAt{};

} // inline namespace Cust

} // namespace Meta
