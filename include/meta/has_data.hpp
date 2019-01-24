

#pragma once

#include <iterator>

namespace meta {

namespace adl_concepts {

using namespace std;

struct has_data {
    template <typename T>
    auto requires_(T&&) -> decltype(data(std::declval<T>()));
};

} // namespace adl_concepts

} // namespace meta
