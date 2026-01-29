// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Dangling_Base_Class.hpp"
#include <mutex>

using namespace Mlib;

DanglingBaseClass::DanglingBaseClass() = default;

DanglingBaseClass::~DanglingBaseClass() {
    assert_no_references();
}

void DanglingBaseClass::print_references() const {
    lerr() << "Remaining locations: " << locs_.size();
    for (const auto& [p, l] : locs_) {
        lerr() << l;
    }
}

void DanglingBaseClass::assert_no_references() const {
    std::shared_lock lock{loc_mutex_};
    if (!locs_.empty()) {
        print_references();
        verbose_abort("Dangling pointers or references remaining");
    }
}

size_t DanglingBaseClass::nreferences() const {
    std::shared_lock lock{loc_mutex_};
    return locs_.size();
}

void DanglingBaseClass::add_source_location(const void* ptr, SourceLocation loc) {
    std::scoped_lock lock{loc_mutex_};
    if (!locs_.try_emplace(ptr, loc).second) {
        verbose_abort("Could not insert source location");
    }
}

void DanglingBaseClass::remove_source_location(const void* ptr) {
    std::scoped_lock lock{loc_mutex_};
    if (locs_.erase(ptr) != 1) {
        verbose_abort("Could not erase source location");
    }
}

const SourceLocation& DanglingBaseClass::loc(const void* ptr) const {
    std::shared_lock lock{loc_mutex_};
    auto it = locs_.find(ptr);
    if (it == locs_.end()) {
        verbose_abort("DanglingBaseClass::loc: Could not find location");
    }
    return it->second;
}
