// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#include <iterator>
#include <type_traits>

#include "DetectedOr.hpp"

namespace Penjing::Meta {

namespace Details {

template< typename It >
using IterValueType = typename std::iterator_traits< It >::value_type;

template< typename It >
using WellDefinedValueType =
    std::bool_constant< !std::is_void_v< IterValueType< It > > >;

} // namespace Details

template< typename T >
using IsIterator =
    DetectedOr< std::false_type, Details::WellDefinedValueType, T >;

} // namespace Penjing::Meta
