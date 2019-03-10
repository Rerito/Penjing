
#pragma once

namespace st {

class access {};

class node_access {
    template <typename>
    friend class insertion_context;

    template <typename Node>
    static void remove_transition(Node& node, typename Node::char_type c) {
        node.tr_.erase(c);
    }

    template <typename Node>
    static void set_parent(Node& tgt, Node const& src) {
        tgt.parent_ = src.parent_;
    }
};
} // namespace st
