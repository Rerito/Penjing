// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <Penjing/Meta/IsDetected.hpp>

namespace Penjing {
namespace SuffixTree {
namespace Concepts {

namespace Details {

template< typename Traits >
using TransitionAllocatorType = typename Traits::TransitionAllocator;

} // namespace Details

template< typename T >
concept NodeTraits = Meta::Detected< Details::TransitionAllocatorType, T >;

} // namespace Concepts
} // namespace SuffixTree
} // namespace Penjing
