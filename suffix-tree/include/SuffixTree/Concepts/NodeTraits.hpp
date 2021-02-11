// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <Meta/IsDetected.hpp>

namespace SuffixTree {
namespace Concepts {

namespace Details {

template< typename Traits >
using TransitionAllocatorType = typename Traits::TransitionAllocator;

}

template< typename T >
concept NodeTraits = Meta::Detected< Details::TransitionAllocatorType, T >;

}
}
