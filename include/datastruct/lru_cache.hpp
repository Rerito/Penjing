
#pragma once


#include "cache.hpp"

class LRUPolicy {
    size_t max_size_;

    // Pop the first inserted element in the cache
    // Does not check for size as this will be done by the caller
    template <typename Cache>
    void pop_first(Cache& cache) const {
        cache.map_to_els_.erase(cache.elems_.front().first);
        cache.elems_.erase(begin(cache.elems_));
    }
public:
    LRUPolicy(size_t max_size) : max_size_(max_size) {}
    
    template <typename Cache>
    void prune(Cache& cache) const {
        // Since C++11 and onward, std::list::size() is guaranteed O(1)
        while (max_size_ < cache.elems_.size()) {
            pop_first(cache);
        }
    }

    template <typename Cache, typename It>
    void refresh_elem(Cache& cache, It el) const {
        cache.elems_.splice(end(cache.elems_), cache.elems_, el);
        auto back_it = --end(cache.elems_);
        cache.map_to_els_[back_it->first] = back_it;
    }
};


template <typename Key, typename Value, typename Hash = std::hash<Key>, typename Eq = std::equal_to<Key> >
using lru_cache = basic_cache<Key, Value, Hash, Eq, LRUPolicy>;
