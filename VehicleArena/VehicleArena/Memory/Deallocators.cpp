// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Deallocators.hpp"
#include <VehicleArena/Memory/Deallocation_Token.hpp>
#include <VehicleArena/Throw_Or_Abort.hpp>

using namespace Mlib;

Deallocators::Deallocators() = default;

Deallocators::~Deallocators() = default;

DeallocationToken Deallocators::insert(const std::function<void()>& deallocate) {
    std::scoped_lock lock{ mutex_ };
    deallocators_.push_front(deallocate);
    return { this, deallocators_.begin() };
}

void Deallocators::erase(const std::list<std::function<void()>>::iterator& token) {
    std::scoped_lock lock{ mutex_ };
    deallocators_.erase(token);
}

void Deallocators::deallocate() {
    std::scoped_lock lock{ mutex_ };
    for (auto& r : deallocators_) {
        r();
    }
}
