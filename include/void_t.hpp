
#pragma once

namespace meta {

template <typename... Args>
struct void_ {
    using type = void;
};

template <typename... Args>
using void_t = typename void_<Args...>::type;

} // namespace meta
