// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

#include <variant>

namespace Meta {

// This class makes it possible for a variant to hold a pointer to "itself".
template< typename... Ts >
struct SelfVariant : std::variant< Ts..., SelfVariant< Ts... >* >
{
    using std::variant< Ts..., SelfVariant< Ts... >* >::variant;
};

}
