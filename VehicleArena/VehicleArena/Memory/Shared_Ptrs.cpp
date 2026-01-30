// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Shared_Ptrs.hpp"
#include <VehicleArena/Memory/Recursive_Deletion.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <mutex>

using namespace VA;

SharedPtrs::SharedPtrs()
    : clearing_{ false }
{}

SharedPtrs::~SharedPtrs() {
    clear();
}

void SharedPtrs::add(std::shared_ptr<Object> ptr) {
    if (clearing_) {
        verbose_abort("SharedPtrs::add called during clearing");
    }
    std::scoped_lock lock{ mutex_ };
    ptrs_.push_back(std::move(ptr));
}

void SharedPtrs::clear() {
    if (clearing_) {
        verbose_abort("SharedPtrs already clearing");
    }
    std::unique_lock lock{ mutex_ };
    clearing_ = true;
    clear_list_recursively_with_lock(ptrs_, lock, [](const auto&) {});
    clearing_ = false;
}
