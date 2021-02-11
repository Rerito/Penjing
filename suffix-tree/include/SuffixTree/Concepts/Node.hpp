// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <functional>
#include <optional>
#include <type_traits>

#include <Meta/IsDetected.hpp>

namespace SuffixTree {
namespace Concepts {

namespace Details {

template< typename T >
using CharType = typename T::CharType;

template< typename T >
using StringViewType = typename T::StringViewType;

template< typename T >
using EdgeType =
    decltype(std::declval< T& >().operator[](std::declval< CharType< T > >()));

template< typename T >
concept HasParent = requires(T& node)
{
    {
        node.parent()
        } -> std::same_as<
            std::optional< std::reference_wrapper< std::add_const_t< T > > > >;
};

template< typename T >
concept HasAddEdge = requires(T& node)
{
    {node.addEdge(std::declval< StringViewType< T > >())};
};

} // namespace Details

template< typename T >
concept Node =
    Details::HasParent< T > && Meta::Detected< Details::CharType, T > &&
    Meta::Detected< Details::StringViewType, T >;

} // namespace Concepts
} // namespace SuffixTree
