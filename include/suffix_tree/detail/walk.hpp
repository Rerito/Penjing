
#include "transition.hpp"

// Default implementation: we must compare all characters along the way down
template <typename RootNode, typename String>
auto walk(RootNode&& node, String const& str) {
    using std::size;
    using std::data;
    size_t i = 0u;
    size_t len = size(str);
    auto strp =  
    while (i < len) {
        auto const& trs = transitions(node);
        auto it = trs.find(str[i]);


    }



}
