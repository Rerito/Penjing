// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <iterator>
#include <ranges>

namespace Penjing {
namespace SuffixTree {
namespace Concepts {

/*!
 * \interface String
 *
 * \brief A concept that defines a String type
 *
 * \detail A String type must be a contiguous_range
 */
template< typename Str >
concept String = std::ranges::contiguous_range< Str > &&
    std::ranges::contiguous_range< Str const >;

} // namespace Concepts
} // namespace SuffixTree
} // namespace Penjing
