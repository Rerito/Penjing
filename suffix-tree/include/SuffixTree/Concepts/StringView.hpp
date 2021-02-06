// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

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
concept StringView = String< Str > && std::ranges::viewable_range< StrView > &&
    std::constructible_from<
        StrView,
        std::ranges::iterator_t< Str >,
        std::ranges::iterator_t< Str > > &&
    std::constructible_from<
        StrView,
        std::ranges::iterator_t< Str const >,
        std::ranges::iterator_t< Str const > >;

}
}
