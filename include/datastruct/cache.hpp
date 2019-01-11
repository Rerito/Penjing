
#pragma once

#include "utils/forward_macro.hpp"

#include <unordered_map>
#include <list>
#include <stdexcept>


namespace detail {

template <typename T>
[[noreturn]] inline T throwing_factory() {
    throw std::runtime_error("No such element in cache");
}

} // namespace detail


template <typename Key, typename Value, typename Hash, typename Eq, typename PruningPolicy>
class basic_cache : PruningPolicy {
    friend PruningPolicy;
public:
    using list_t = std::list<std::pair<Key,Value> >;
    using list_iterator = typename list_t::iterator;
    using map_t = std::unordered_map<Key, list_iterator, Hash, Eq>;
    using map_value_t = typename map_t::value_type;

private:
    list_t elems_;
    map_t map_to_els_;

public:
    using PruningPolicy::PruningPolicy;

    template <typename... Args>
    void emplace(Key const& k, Args&&... args) {
        auto& it = map_to_els_[k];
        if (it != list_iterator {}) {
            throw std::invalid_argument("k");
        }
        elems_.emplace_back(std::pair<Key, Value>(k, Value(CPPFWD(args)...)));
        it = --end(elems_);
        PruningPolicy::prune(*this);
    }

    template <typename ElemFactory>
    Value& get(Key const& k, ElemFactory&& factory) {
        auto& el = map_to_els_[k];
        if (el == list_iterator {}) {
            emplace(k, CPPFWD(factory)());
        }
        PruningPolicy::refresh_elem(*this, el);
        return el->second;
    }

    Value& get(Key const& k) {
        return get(k, &detail::throwing_factory<Value>);
    }

    void erase(Key const& k) {
        auto it = map_to_els_.find(k);
        if (end(map_to_els_) != it) {
            elems_.erase(it->second);
            map_to_els_.erase(it);
        } else {
            throw std::invalid_argument("k");
        }
    }
};


class NoPruning {
public:
    template <typename Cache>
    void prune(Cache&) const noexcept {}

    template <typename Cache, typename It>
    void refresh_elem(Cache&, It const&) const noexcept {}
};


template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Eq = std::equal_to<Key> >
using cache = basic_cache<Key, Value, Hash, Eq, NoPruning>;
