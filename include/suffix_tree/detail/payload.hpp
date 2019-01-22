
#pragma once

#include <utility>
#include <type_traits>

template <typename String, typename Value>
using payload_t = std::conditional_t<
    std::is_void<Value>::value,
    String,
    std::pair<String, Value>
>;


template <typename Value>
struct payload_ {
    template <typename T>
    Value& get_payload(T& el) const noexcept {
        using std::get;
        return get<1>(el);
    }

    template <typename T>
    Value const& get_payload(T const& el) const noexcept {
        using std::get;
        return get<1>(el);
    }

    template <typename T>
    auto& get_str(T& el)  const noexcept{
        using std::get;
        return get<0>(el);
    }
    
    template <typename T>
    auto const& get_str(T const& el)  const noexcept{
        using std::get;
        return get<0>(el);
    }
};

template <>
struct payload_<void> {
    template <typename T>
    void get_payload(T const&) const noexcept {}

    template <typename T>
    auto& get_str(T& el) {
        return el;
    }

    template <typename T>
    auto const& get_str(T const& el) {
        return el;
    }
};
