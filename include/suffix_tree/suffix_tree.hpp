
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
    using transition_type = typename node_type::transition_type;
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
    char_type end_token_;
    string_cache_type str_cache_;
    std::vector<node_type*> leaves_;
    std::unique_ptr<node_type> root_;
};

