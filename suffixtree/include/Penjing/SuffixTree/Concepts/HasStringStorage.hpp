// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>

#include <Penjing/Meta/Access.hpp>
#include <Penjing/Meta/IsDetected.hpp>

#include "../Algorithm/MutatingTreeAlgorithm.hpp"

namespace Penjing {
namespace SuffixTree {
namespace Concepts {

namespace Details {

template< typename T >
using StringStorageType = decltype(std::declval< T >().mutableStringStorage(
    std::declval< Meta::Access< Algorithm::MutatingTreeAlgorithm > const& >()));

} // namespace Details

template< typename T >
concept HasStringStorage = Meta::Detected< Details::StringStorageType, T >;

} // namespace Concepts
} // namespace SuffixTree
} // namespace Penjing
