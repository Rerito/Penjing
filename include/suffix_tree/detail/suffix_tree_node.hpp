
#pragma once


#include <unordered_map>
#include <string>
#include <sstream>
#include <memory>
#include "utils/memory.hpp"

namespace st {

template  <typename String>
struct string_bundle {
    std::reference_wrapper<String> str_;
    size_t str_idx_;
};

template <typename Node>
struct transition {
    using node_type = Node;
    using sview_type = typename Node::sview_type;
    using node_ptr = memory::custom_alloc_unique_ptr<node_type, typename node_type::allocator>;
    template <typename T>
    friend std::string to_string(transition<T> const&);
    node_ptr dest_;
    sview_type sub_str_;

    transition(node_ptr p, sview_type const& sstr) : dest_(std::move(p)), sub_str_(sstr) {}
    bool is_valid() const {
        return !!dest_;
    }
};

template <typename T>
std::string to_string(transition<T> const& t) {
    using std::to_string;
    using std::data;
    using std::size;
    std::stringstream sstr;
    sstr << "(size: " << size(t.sub_str_) << ", value: " << t.sub_str_ << ") -> " << (t.dest_.get());
    return sstr.str();
}

template <typename String, typename SView>
class suffix_tree_node {
    template <typename, typename, typename>
    friend class suffix_tree;
public:
    using char_type = typename String::value_type;
    using string_type = String;
    using sview_type  = SView;
    using self_type = suffix_tree_node;
    using allocator = std::allocator<self_type>;
    using transition_type = transition<self_type>;

    // Disable the copy for now...
    suffix_tree_node() : parent_(nullptr), link_(nullptr), tr_() {}
    suffix_tree_node(suffix_tree_node const&) = delete;
    suffix_tree_node& operator=(suffix_tree_node const&) = delete;

    suffix_tree_node(suffix_tree_node&&) = default;
    suffix_tree_node& operator=(suffix_tree_node&&) = default;

    bool operator==(suffix_tree_node const& other) const {
        bool value = (size(tr_) == size(other.tr_));
        if (value) {
            for (auto const& t : tr_) {
                auto const& t_other = other.find_transition(t.first);
                if (!t_other.is_valid() || t_other.sub_str_ != t.second.sub_str_) {
                    return false;
                } else {
                    value = value && (*t.second.dest_ == *t_other.dest_);
                }
            }
        }
        return value;
    }

    bool operator!=(suffix_tree_node const& other) const {
        return !(*this == other);
    }


    transition_type const& find_transition(char_type c) const {
        auto it = tr_.find(c);
        if (end(tr_) == it) {
            return empty_transition();
        }
        return it->second;
    }
    transition_type& find_transition(char_type c) {
        return const_cast<transition_type&>(
            const_cast<suffix_tree_node const&>(*this).find_transition(c)
        );
    }

private:
    static transition_type& empty_transition() {
        static transition_type empty_tr(typename transition_type::node_ptr(nullptr), sview_type(nullptr, 0));
        return empty_tr;
    }

    suffix_tree_node *parent_; // The parent node for the current substring
    suffix_tree_node *link_;
    // The node has ownership of its children but not of its parent nor its
    // link
    std::unordered_map<char_type, transition<self_type> > tr_; // The transitions to child nodes
};

} // namespace st
