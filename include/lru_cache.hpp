
#pragma once

#include "forward_macro.hpp"

#include <unordered_map>
#include <list>
#include <stdexcept>

namespace detail {

template <typename T>
[[noreturn]] inline T throwing_factory() {
    throw std::runtime_error("No such element in cache");
}

} // namespace detail

template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Eq = std::equal_to<Key> >
class lru_cache {
public:
    using list_t = std::list<std::pair<Key,Value> >;
    using list_iterator = typename list_t::iterator;
    using map_t = std::unordered_map<Key, list_iterator, Hash, Eq>;
    using map_value_t = typename map_t::value_type;

private:
    size_t max_size_;
    list_t elems_;
    map_t map_to_els_;


    // Pop the first inserted element in the cache
    // Does not check for size as this will be done by the caller
    void pop_first() {
        map_to_els_.erase(elems_.front().first);
        elems_.erase(begin(elems_));
    }

    void prune() {
        // Since C++11 and onward, std::list::size() is guaranteed O(1)
        while (max_size_ < elems_.size()) {
            pop_first();
        }
    }

    void refresh_elem(list_iterator el) {
        elems_.splice(end(elems_), elems_, el);
        auto back_it = --elems_.end();
        map_to_els_[back_it->first] = back_it;
    }

public:
    lru_cache(size_t max_size) : max_size_(max_size), elems_(), map_to_els_() {}

    template <typename... Args>
    void emplace(Key const& k, Args&&... args) {
        auto& it = map_to_els_[k];
        if (it != list_iterator {}) {
            throw std::invalid_argument("k");
        }
        elems_.emplace_back(std::pair<Key, Value>(k, Value(CPPFWD(args)...)));
        it = --end(elems_);
        prune();
    }

    template <typename ElemFactory>
    Value& get(Key const& k, ElemFactory&& factory) {
        auto& el = map_to_els_[k];
        if (el == list_iterator {}) {
            emplace(k, CPPFWD(factory)());
        }
        refresh_elem(el);
        return el->second;
    }

    Value& get(Key const& k) {
        return get(k, &detail::throwing_factory<Value>);
    }
};
