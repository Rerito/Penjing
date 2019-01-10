

#pragma once

#include <unordered_map>
#include <memory>

namespace detail {

struct set_node {
    set_node *parent;
    size_t rank;

    set_node() : rank(0u), parent(nullptr) {}
};

class PathHalving {
    bool do_reduce_;
public:
    PathHalving() : do_reduce_(true) {}

    void operator()(set_node& node) noexcept {
        if (do_reduce_ && node.parent && node.parent->parent) {
            node.parent = node.parent->parent;
        }
        do_reduce_ = !do_reduce_;
    }
};

struct FullPathReduction {
    void operator()(set_node& node) const noexcept {
        if (node.parent && node.parent->parent) {
            node.parent = node.parent->parent;
        }
    }
};

template <bool KeepFirst=true>
void make_union(set_node& n1, set_node& n2) {
    if (&n1 != &n2) {
        bool switch_ref = n1.rank < n2.rank || (!KeepFirst && n1.rank == n2.rank);
        auto& n1_ = switch_ref ? n2 : n1;
        auto& n2_ = switch_ref ? n1 : n2;
        ++n1_.rank;
        n2_.parent = &n1_;
    }
}

template <typename PathReductionPolicy>
set_node& find(set_node& el, PathReductionPolicy&& reduce = PathHalving{}) {
    set_node *repr = &el;
    while (repr->parent) {
        reduce(*repr);
        repr = repr->parent;
    }
    return *repr;
}

} // namespace detail

template <typename Key, typename Hash = std::hash<Key>, typename Eq = std::equal_to<Key>, typename PathReductionPolicy = detail::PathHalving >
class disjoint_sets {
public:
    using node_type = detail::set_node;
private:
    std::unordered_map<Key, std::unique_ptr<node_type>, Hash, Eq> repr_;

public:
    node_type& make_set(Key const& k) {
        auto& pNode = repr_[k];
        if (pNode) {
            throw std::runtime_error("Element with key k already registered in disjoint_sets");
        }
        pNode = std::make_unique<node_type>();
        return *pNode;
    }

    template <bool KeepFirst=true>
    void make_union(Key const& k1, Key const& k2) {
        using detail::make_union;
        make_union<KeepFirst>(find(k1), find(k2));
    }

    node_type& find(Key const& k) {
        using detail::find;
        auto it = repr_.find(k);
        if (end(repr_) == it) {
            throw std::runtime_error("No element for the given key in the set");
        }
        return find(*(it->second), PathReductionPolicy{});
    }
    
    bool is_in_same_set(Key const& k1, Key const& k2) {
        return Eq{}(k1, k2) || &find(k1) == &find(k2);
    }
    
    void clear() {
        repr_.clear();
    }
};


