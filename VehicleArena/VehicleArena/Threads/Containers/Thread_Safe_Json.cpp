// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Thread_Safe_Json.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <mutex>
#include <stdexcept>

using namespace VA;

ThreadSafeJson::ThreadSafeJson() = default;

ThreadSafeJson::ThreadSafeJson(const ThreadSafeJson& other) {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    j_ = other.j_;
}

ThreadSafeJson::ThreadSafeJson(ThreadSafeJson&& other) noexcept {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    j_ = std::move(other.j_);
}

ThreadSafeJson& ThreadSafeJson::operator = (const ThreadSafeJson& other) {
    *this = (nlohmann::json)other;
    return *this;
}

ThreadSafeJson& ThreadSafeJson::operator = (nlohmann::json other) {
    std::scoped_lock lock{ mutex_ };
    j_ = std::move(other);
    return *this;
}

bool ThreadSafeJson::operator == (const ThreadSafeJson& other) const {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    return j_ == other.j_;
}

bool ThreadSafeJson::operator != (const ThreadSafeJson& other) const {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    return j_ != other.j_;
}

std::partial_ordering ThreadSafeJson::operator <=> (const ThreadSafeJson& other) const {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    return j_ <=> other.j_;
}

nlohmann::json ThreadSafeJson::json() const {
    std::scoped_lock lock{ mutex_ };
    return j_;
}

void VA::from_json(const nlohmann::json& j, ThreadSafeJson& t) {
    t = j;
}

void VA::to_json(nlohmann::json& j, const ThreadSafeJson& t) {
    j = t.json();
}
