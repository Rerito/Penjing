
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

    bool is_substring(string_type const& str) {
        using std::data;
        using std::size;
        reference_point rp { root_.get(), sview_type(data(str), size(str)) };
        get_starting_node(rp);
        return !size(rp.start_);
    }

private:
    
    struct reference_point {
        node_type *node_;
        sview_type start_;

        template <typename T, typename U>
        operator std::tuple<T&, U&> () {
            return std::forward_as_tuple(node_, start_);
        }
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
            if (tr.is_valid()) {
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
                } else {
                    break;
                }
                exhausted = (str_ptr == str_end);
            } else {
                break;
            }
        }
        rp.start_ = sview_type(str_ptr, str_end - str_ptr);
    }


    reference_point canonize(node_type* n, sview_type const& substr) {
        // Use the skip/count trick to avoid checking every character on the
        // transition substring...
        using std::data;
        using std::size;
        reference_point rp {n, substr};
        if (!size(substr)) {
        } else {
            auto total = size(substr);
            decltype(total) current = 0u; 
            auto substr_ptr = data(substr);
            auto tr_rw = std::ref(n->find_transition(substr[0]));
            if (!tr_rw.get().is_valid()) {
                goto return_label;
            }
            auto delta = size(tr_rw.get().sub_str_);
            while (delta + current <= total) {
                substr_ptr += delta + 1;
                current += delta + 1;
                if (current < total) {
                    n = tr_rw.get().dest_.get();
                    tr_rw = n->find_transition(*substr_ptr);
                    if (!tr_rw.get().is_valid()) {
                        break;
                    }
                    delta = size(tr_rw.get().sub_str_);
                }
            }
            rp = { n, sview_type(substr_ptr, &substr.back() - substr_ptr + 1) };
        }
return_label:
        return rp;
    }

    std::pair<bool, node_type*> test_and_split(node_type *n, sview_type const& str, char_type const& t) { 
        using std::data;
        using std::size;
        if (!size(str)) {
            return std::make_pair(n->find_transition(t).is_valid(), n);
        } else {
            auto& tr = n->find_transition(str[0]);
            if (tr.sub_str_[size(str)] == t) {
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
                    data(tr.sub_str_) + size(str), // starts at x
                    size(tr.sub_str_) - size(str)
                );
                // Link the new state to the end of the transition
                auto [new_tr_it, did_emplace] = r->tr_.emplace(
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

    reference_point update(node_type *n, sview_type const& substr, sview_type const& whole_str) {
        using std::size;
        using std::data;
        auto end_wstr = data(whole_str) + size(whole_str);
        auto b_substr = data(substr);
        // substr is never empty here
        auto trunc_substr = sview_type(data(substr), size(substr) - 1);
        auto oldr = root_.get();
        auto [is_endpoint, r] = test_and_split(n, trunc_substr, substr.back());
        while (!is_endpoint) {
            using memory::make_unique;
            // Add a leaf to the suffix tree
            auto [leaf_tr_it, did_emplace] = r->tr_.emplace(
                substr.back(),
                transition(
                    make_unique<node_type, node_allocator>(),
                    sview_type(
                        &substr.back(),
                        end_wstr - &substr.back()
                    )
                )
            );
            leaf_tr_it->second.dest_->parent_ = r;
            if (oldr != root_.get()) {
                oldr->link_ = r;
            }
            auto link = n->link_;
            if (nullptr == link) {
                // n is the root...
                // We need to unroll the behavior of the special state
                // defined in Ukkonen's paper
                link = n;
                if (size(trunc_substr)) {
                    trunc_substr = sview_type(data(trunc_substr) + 1, size(trunc_substr) - 1);
                } else {
                    trunc_substr = sview_type(data(trunc_substr) + 1, 0u);
                    is_endpoint = true;
                    break;
                }
            }
            std::tie(n, trunc_substr) = canonize(link, trunc_substr);
            std::tie(is_endpoint, r) = test_and_split(n, trunc_substr, substr.back());
        }
        if (oldr != root_.get()) {
            oldr->link_ = r;
        }
        return { n, trunc_substr };
    }


    void deploy_suffixes(string_type const& whole_str) {
        using std::size;
        using std::data;
        reference_point rp { root_.get(), sview_type(data(whole_str), size(whole_str)) };
        get_starting_node(rp);
        auto str_ptr = data(rp.start_);
        auto str_size = size(rp.start_);
        // We have the starting point, we must perform the remaining extensions
        for (decltype(str_size) i = 1u; i < str_size; ++i) {
            rp = update(
                rp.node_,
                sview_type(
                    data(rp.start_),
                    i - (data(rp.start_) - str_ptr)
                ),
                sview_type(data(whole_str), size(whole_str))
            );
            rp = canonize(rp.node_, rp.start_);
        }
    }

    // Data members
    char_type end_token_;
    string_cache_type str_cache_;
    std::vector<node_type*> leaves_;
    node_ptr root_;
};

} // namespace st
