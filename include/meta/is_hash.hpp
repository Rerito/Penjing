
#pragma once

#include <type_traits>
#include <utility>

namespace meta {

template <typename Hashable>
struct is_hash {
    template <typename T>
    auto requires_(T&& t) -> std::enable_if_t<
        std::is_convertible<
            decltype(t(std::declval<Hashable>())),
            std::size_t
        >::value
    >;
};

} // namespace meta
