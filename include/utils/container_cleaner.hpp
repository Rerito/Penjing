
#pragma once

#include <functional>
#include <vector>
#include "forward_macro.hpp"



// This little structure will help exception safe design
// By removing elements inserted in a work container through object destruction
// This will come in handy to do clean up should things go south in the middle
// of an insertion in a suffix tree for instance.
template <typename Ctnr, typename EraseArg=typename Ctnr::iterator>
class container_cleaner {
    std::reference_wrapper<Ctnr> c_;
    std::vector<EraseArg> elems_to_clean_;

public:
    container_cleaner(Ctnr& c) : c_(c), elems_to_clean_() {}
    ~container_cleaner() {
        for (auto& e : elems_to_clean_) {
            c_.get().erase(e);
        }
    }

    template <typename... Args>
    void add_to_clean(Args&&... args) {
        elems_to_clean_.emplace_back(CPPFWD(args)...);
    }

    void clear() {
        elems_to_clean_.clear();
    }
};

// This version simply cleans out all the registered container
template <typename Ctnr>
class container_wiper {
    bool do_clear_;
    std::reference_wrapper<Ctnr> c_;
public:
    container_wiper(Ctnr& c) : do_clear_(true), c_(c) {}
    ~container_wiper() {
        if (do_clear_) {
            c_.get().clear();
        }
    }

    void clear() {
        do_clear_ = false;
    }
};

