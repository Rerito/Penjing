
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
};
} // namespace st
