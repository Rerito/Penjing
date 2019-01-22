
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
    using hasher_type = StrHash;
    using string_cache_type = cache<size_t, string_type>;

    suffix_tree(char_type const& end_token) :
        end_token_(end_token),
        str_cache_(),
        leaves_(),
        root_(std::make_unique<node_type>()) {}

    template <typename... Args>
    void emplace(string_type str_in, Args&&... args) {
        using std::size;
        container_cleaner<string_cache_type, size_t> clnr;
        if (end_token_ != str_in[size(str_in)-1]) {
            throw std::invalid_argument("The given string doesn't end with the end token of this tree");
        }
        size_t str_key = get_index(str_in);
        auto& str = str_cache_.emplace(str_key, std::move(str_in));
        clnr.add_to_clean(str_key);
        // We are now ready to perform insertion in the suffix tree
        // Once this is done, we can finally add the payload.
        clnr.clear();
    }

private:

    struct reference_point {
        node_type *node_;
        sview_type start_;
    };

    std::pair<bool, node_type*> test_and_split(node_type *n, sview_type sv, char_type t) {
        using std::size;
        using std::data;
        if (!size(sv)) {
            return std::make_pair(n->find_transition(t).get().is_valid(), n);
        } else {
            auto& tr = n->find_transition(sv[0]);
            auto i_t = size(sv) + 1;
            // Test and split is always called with canonical pairs (n, sw),
            // This means that by walking down from n with sw, we do not
            // traverse any explicit state...
            
            if (tr.sub_str_[i_t] == t) {
                return std::make_pair(true, n);
            } else {
                // The transition from n starting with sw do not match t at
                // character size(sw)+1, thus we need to branch!
                auto r = std::make_unique<node_type>();
                auto r_raw = r.get();
                r->parent_ = n;
                tr.dest_->parent = r_raw;
                tr.get().sub_str_ = sview_type(data(tr.get().sub_str_), size(sv));
                r->tr_[(tr.sub_str_[size(sv)])] = typename node_type::transition_type(
                    std::move(tr.dest_),
                    sview_type(data(tr.sub_str_) + size(sv), size(tr.sub_str_) - size(sv))
                );
                tr.dest_ = std::move(r);
                return std::make_pair(false, r_raw);
            }
        }
    }

    reference_point update(node_type *n, sview_type const& substr, sview_type const& whole_str) {
        using std::data;
        using std::size;
        // Update is never called with an empty substr string view
        auto trunc_substr = sview_type(
            data(substr),
            size(substr) - 1
        );
        auto r_old = root_.get();
        auto [is_endpoint, r] = test_and_split(n, trunc_substr, substr.back());
        while (!is_endpoint) {
            // We must create a new leaf from r...
            // When we do so, substr is always a string view on the string we
            // are currently inserting in the tree...
            // Let's leverage on that to create the correct leaf transition
            auto tr_sv_data = data(substr) + size(substr);
            auto tr_sv_size = size(whole_str) + data(whole_str) - tr_sv_data;
            r->tr[substr.back()] = typename node_type::transition_type(
                std::make_unique<node_type>(),
                sview_type(tr_sv_data, tr_sv_size) // TODO keep track of actual string size...
            );
            if (r_old != root_.get()) {
                r_old->link_ = r;
            }
            r_old = r;
            auto rp = canonize(n->link_, trunc_substr);
            auto ep_r = test_and_split(rp.node_, rp.start_, substr.back());
            r = ep_r.second;
            is_endpoint = ep_r.first;
        }
        return { n, substr };
    }

    reference_point canonize(node_type *node, sview_type const& substr) {
        using std::data;
        using std::size;
        auto psstr = data(substr);
        if (!size(substr)) {
            return { node, sview_type(psstr, 0) };
        }
        auto tr = node->find_transition(*psstr);
        size_t remainder = size(substr), delta;
        while((delta = size(tr.get().sub_str_)) < remainder) {
            remainder -= 1 + delta;
            psstr += 1 + delta;
            node = tr.get().target();
            if (!remainder) {
                break;
            }
            tr = node->find_transition(*psstr);
        }
        return { node, sview_type(psstr, remainder) };
    }

    void get_starting_node(string_type const& str, reference_point& active_point) {
        // Walk down the tree using `str`..., starting from `active_point.node`
        // This function is always called with active_point.start_ being a
        // string_view of str...
        using std::data;
        using std::size;
        auto s_len = size(str);
        auto end_str = data(str) + s_len;
        bool s_ranout = false;
        while (!s_ranout) {
            auto start_ = data(active_point.start_);
            if (start_ >= end_str) {
                break;
            }
            auto tr = active_point.node_->find_transition(*start_);
            if (!(tr.get().is_valid())) {
                return;
            }
            auto tr_view_ptr = data(tr.get().sub_str_);
            auto end_view_ptr = tr_view_ptr + size(tr.get().sub_str_);
            for (++start_, ++tr_view_ptr;
                 start_ != end_str && tr_view_ptr != end_view_ptr;
                 ++start_, ++tr_view_ptr) {
                if (*(start_) != *(tr_view_ptr)) {
                    active_point.start_ = sview_type(start_, size(active_point.start_) - (start_ - active_point.start_));
                    return;
                }
                active_point.start_ = sview_type(start_, size(active_point.start_) - (start_ - active_point.start_));
            }
            if (tr_view_ptr == end_view_ptr) {
                active_point.node_ = tr.get().dest_;
            }
        }
        throw std::runtime_error("");
    }

    void deploy_suffixes(string_type const& str) {
        using std::data;
        using std::size;
        auto whole_str = sview_type(data(str), size(str));
        reference_point active_point { root_.get(), whole_str };
        get_starting_node(str, active_point);
        for (auto p = data(active_point.start_); p != data(str) + size(str) + 1; ++p) {
            active_point = update(
                active_point.node_,
                sview_type(
                  data(active_point.start_),
                  p - data(active_point.start_) + 1
                ),
                whole_str
            ); 
            active_point = canonize(active_point.node_, active_point.start_);
        }
    }

    size_t get_index(string_type const& str) {
        return hasher_type()(str);
    }
    char_type end_token_;
    string_cache_type str_cache_;
    std::vector<node_type*> leaves_;
    std::unique_ptr<node_type> root_;
};

