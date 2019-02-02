
#pragma once

#include <vector>
#include <stack>
#include <variant>

#include "access.hpp"

namespace st {

class mock_insertion_context {
public:
    template <typename Node, typename CharT>
    void add_leaf_op(Node *, CharT const&) noexcept{}

    template <typename Node, typename CharT>
    void add_branch_op(Node*, CharT const&, CharT const&) noexcept {}

    void cancel() noexcept {}
};

template <typename Node>
class insertion_context {
public:
    using node_type = Node;
    using char_type = typename Node::char_type;
    using sview_type = typename Node::sview_type;
private:
    // Operation structures
    class add_leaf {
        Node *parent_;
        char_type c_;
    public:

        add_leaf(Node *p, char_type const& c) : parent_(p), c_(c) {}
        void undo() {
            node_access::remove_transition(*parent_, c_);
        }
    };

    class new_branch {
        Node *orig_;
        char_type jmp1_;
        char_type jmp2_;
    public:
        new_branch(Node* orig, char_type const& j1, char_type const& j2) : orig_(orig), jmp1_(j1), jmp2_(j2) {}
        void undo() {
            using std::size;
            using std::data;
            auto& tr = orig_->find_transition(jmp1_);
            auto& tr2 = tr.dest_->find_transition(jmp2_);
            tr.dest_ = std::move(tr2.dest_);
            tr.sub_str_ = sview_type(data(tr.sub_str_), size(tr.sub_str_) + size(tr2.sub_str_));
        }
    };

    using op_variant = std::variant<add_leaf, new_branch>;
public:

    void add_leaf_op(node_type *leaf_parent, char_type t_head) {
        ops_.emplace(std::in_place_type<add_leaf>,  leaf_parent, t_head);
    }

    void add_branch_op(node_type *orig, char_type const& j1, char_type const& j2) {
        ops_.emplace(std::in_place_type<new_branch>, orig, j1, j2);
    }

    ~insertion_context() {
        while(!ops_.empty()) {
            std::visit([](auto& op) { op.undo(); }, ops_.top());
            ops_.pop();
        }
    }

    void cancel() {
        ops_ = {};
    }
private:
    std::stack<op_variant, std::vector<op_variant> > ops_;
};

} // namespace st
