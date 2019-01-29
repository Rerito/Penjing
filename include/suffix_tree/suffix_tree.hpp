
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_set>
#include <functional>

#include "datastruct/cache.hpp"
#include "utils/container_cleaner.hpp"
#include "detail/suffix_tree_node.hpp"
#include "detail/payload.hpp"

namespace st {

namespace detail {

} // namespace detail

template <typename String=std::string,
          typename SView=std::basic_string_view<
              typename String::value_type,
              typename String::traits_type>,
          typename StrHash = std::hash<String>
         >
class suffix_tree {
public:
    using string_type = String;
    using index_type = size_t; // TODO: Update this to match correct index type used here
    using char_type = typename String::value_type;
    using sview_type  = SView;
    using node_type   = suffix_tree_node<String, SView>;
    using node_allocator = typename node_type::allocator;
    using node_ptr = memory::custom_alloc_unique_ptr<node_type, node_allocator>;
    using transition_type = typename node_type::transition_type;
    using hasher_type = StrHash;
    using string_cache_type = cache<size_t, string_type>;

    suffix_tree(char_type const& end_token) :
        end_token_(end_token),
        str_cache_(),
        leaves_(),
        root_(memory::make_unique<node_type, node_allocator>()) {}

    template <typename... Args>
    void emplace(string_type str_in, Args&&... args) {
        using std::size;
        if (end_token_ != str_in[size(str_in)-1]) {
            throw std::invalid_argument("The given string doesn't end with the end token of this tree");
        }
        size_t str_key = get_index(str_in);
        auto& str = str_cache_.emplace(str_key, std::move(str_in));
        deploy_suffixes(str);
    }

private:

    struct reference_point {
        node_type *node_;
        sview_type start_;
    };

    size_t get_index(string_type const& str) {
        return hasher_type()(str);
    }


    void get_starting_node(reference_point& rp) {
        using std::data;
        using std::size;
        auto str_ptr = data(rp.start_);
        auto str_size = size(rp.start_);
        auto str_end = str_ptr + str_size;
        bool exhausted = !str_size;
        while (!exhausted) {
           auto& tr = rp.node_->find_transition(*str_ptr);
           auto trstr_ptr = data(tr.sub_str_) + 1;
           ++str_ptr;
           for (;
                (trstr_ptr != data(tr.sub_str_) + size(tr.sub_str_)) &&
                (str_ptr != str_end);
                ++trstr_ptr, ++str_ptr) {
               if (*trstr_ptr != *str_ptr) {
                  break;
               }
           }
           if (trstr_ptr == data(tr.sub_str_) + size(tr.sub_str_)) {
              rp.node_ = tr.dest_.get();
           }
           exhausted = (str_ptr == str_end);
        }
        rp.start_ = sview_type(str_ptr, str_end - str_ptr);
    }


    reference_point canonize(node_type* n, sview_type const& substr) {
        // Use the skip/count trick to avoid checking every character on the
        // transition substring...
        using std::data;
        using std::size;
        if (!size(substr)) {
            return {n, substr};
        } else {
            auto remainder = size(substr);
            auto substr_ptr = data(substr);
            auto tr_rw = std::ref(n->find_transition(substr[0]));
            auto delta = size(tr_rw.get().sub_str_);
            while (delta <= remainder) {
                substr_ptr += delta + 1;
                remainder -= delta + 1;
                if (remainder) {
                    n = tr_rw.get().dest_.get();
                    tr_rw = n->find_transition(*substr_ptr);
                }
            }
            return {n, sview_type(substr_ptr, remainder)};
        }
    }

    std::pair<bool, node_type*> test_and_split(node_type *n, char_type const& t, sview_type const& str) { 
        using std::data;
        using std::size;
        if (!size(str)) {
            return std::make_pair(n->find_transition(t).is_valid(), n);
        } else {
            auto& tr = n->find_transition(t);
            if (tr.sub_str_[size(str)+1] == t) {
                return { true, n };
            } else {
                // Let a.x.b be the transition substring where a and b are
                // substring (b is possibly empty) and x a single character...
                // We must create a new state from n with a transition labeled a
                // And link it to existing state transition(n, a.x.b).dest_ with transition
                // labeled x.b
                using memory::make_unique;
                auto r = make_unique<node_type, node_allocator>();
                r->parent_ = n;
                // Let's define the label for the transition from r to tr dest_
                auto t_end = sview_type(
                    data(tr.sub_str_) + size(str) + 1, // starts at x
                    size(tr.sub_str_) - size(str) - 1
                );
                // Link the new state to the end of the transition
                r->tr_.emplace(
                    t_end[0],
                    transition(std::move(tr.dest_), t_end)
                );
                // And now edit the former transition so that it points to r
                // with the proper label.
                tr.sub_str_ = sview_type(data(tr.sub_str_), size(str));
                tr.dest_ = std::move(r);
                return { false, tr.dest_.get() };
            }
        }
    }


    void deploy_suffixes(string_type const& whole_str) {
        using std::size;
        using std::data;
        reference_point rp { root_.get(), sview_type(data(whole_str), size(whole_str)) };
        get_starting_node(rp);
    }

    // Data members
    char_type end_token_;
    string_cache_type str_cache_;
    std::vector<node_type*> leaves_;
    node_ptr root_;
};

} // namespace st
