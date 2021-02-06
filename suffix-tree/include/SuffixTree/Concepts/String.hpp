
#pragma once

#include <concepts>
#include <iterator>
#include <ranges>

namespace SuffixTree {
namespace Concepts {

namespace Details {

template< typename T >
using BeginType = decltype(std::ranges::begin(std::declval< T& >()));

template< typename T >
using EndType = decltype(std::ranges::end(std::declval< T& >()));

template< typename Str >
concept String =
    std::contiguous_iterator< BeginType< Str > >
    && std::same_as< BeginType< Str >, EndType< Str > >;

}

/*!
 * \interface String
 *
 * \brief A concept that defines a String type
 *
 * \detail A String type must have consistent begin and end semantics with a
 *         return type that satisfies std::contiguous_iterator
 */
template< typename Str >
concept String = Details::String< Str > && Details::String< Str const >;

}
}
