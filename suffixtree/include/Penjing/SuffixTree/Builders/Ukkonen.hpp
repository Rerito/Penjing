// Copyright (c) 2022, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include "TreeBuild.hpp"
#include "Ukkonen/Build.hpp"

namespace Penjing {
namespace SuffixTree {

namespace CPO {

struct Ukkonen
{
    template< typename Tree, typename... Args >
    constexpr void operator()(Tree& tree, Args&&... args) const noexcept(
        noexcept(build(Builders::ukkonen, tree, std::forward< Args >(args)...)))
    {
        build(Builders::ukkonen, tree, std::forward< Args >(args)...);
    }
};

} // namespace CPO

inline namespace Cust {

// Convenience alias for the generic ukkonen tree build.
inline constexpr CPO::Ukkonen ukkonenBuild = {};

} // namespace Cust

} // namespace SuffixTree
} // namespace Penjing
