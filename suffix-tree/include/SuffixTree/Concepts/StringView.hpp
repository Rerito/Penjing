
#pragma once

#include "String.hpp"

namespace SuffixTree {
namespace Concepts {

/*!
 * \interface StringView
 *
 * \brief A concept that defines the requirements for a StringView type
 *
 * \detail A `StringView< Str >` type must be constructible using iterators from
 *         the String `Str` type and must be a viewable range.
 */
template< typename Str, typename StrView >
concept StringView =
    String< Str > &&
    std::constructible_from<
        StrView,
        Details::BeginType< Str >,
        Details::BeginType< Str > > &&
    std::constructible_from<
        StrView,
        Details::BeginType< Str const >,
        Details::BeginType< Str > const >;

}
}
