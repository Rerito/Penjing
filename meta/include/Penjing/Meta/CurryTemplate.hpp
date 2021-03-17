// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

namespace Penjing {
namespace Meta {

// Curry the application of the given template template parameter.
// Given a template class Z, this defines the "curried" metafunction f
// which given the parameter pack Args... returns the type Z< Args... >
template< template< typename... > class Z >
struct CurryTemplate
{
    template< typename... Args >
    using f = Z< Args... >;
};

} // namespace Meta
} // namespace Penjing
