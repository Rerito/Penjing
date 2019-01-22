
#pragma once


#include <unordered_map>
#include <memory>
#include <stack>

namespace detail {

template <typename UPtrMap>
void release_map(UPtrMap& map) {
    for (auto& el : map) {
        el.release(); // release the unique pointers
    }
}

} // namespace detail

template <typename Alloc>
struct alloc_deleter {
    void operator()(void *p) {
        Alloc{}.deallocate(p, sizeof (typename Alloc::value_type));
    }
};




template <typename Node>
Node *copy_node(Node const * const node) {
    using allocator_type = typename Node::allocator_type;
    using node_uptr = std::unique_ptr<Node, alloc_deleter<std::allocator<Node> > >;
    std::unordered_map<Node*, node_uptr> nodes_mapping;
    allocator_type al;

    auto *pNode = nodes_mapping[node].get();
    detail::release_map(nodes_mapping);
    return pNode;
}
