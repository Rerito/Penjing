
#pragma once

#include <iterator>

namespace meta {

namespace adl_concepts {

using namespace std;

struct has_size {
    template <typename T>
    auto requires_(T&&) -> decltype(size(std::declval<T>()));
};

} // namespace adl_concepts

} // namespace meta
