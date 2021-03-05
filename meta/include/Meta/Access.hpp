// Copyright (c) 2021, Rerito
// SPDX-License-Identifier: MIT

#pragma once

namespace Meta {

template< typename T >
class Access
{
    friend T;
    Access() = default;
    Access(Access&&) = default;
};

} // namespace Meta
