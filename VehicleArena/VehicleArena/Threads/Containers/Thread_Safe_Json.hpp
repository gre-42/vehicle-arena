// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <compare>
#include <nlohmann/json.hpp>

namespace VA {

class ThreadSafeJson {
public:
    ThreadSafeJson();
    ThreadSafeJson(const ThreadSafeJson& other);
    ThreadSafeJson(ThreadSafeJson&& other) noexcept;
    ThreadSafeJson& operator = (const ThreadSafeJson& other);
    ThreadSafeJson& operator = (nlohmann::json other);
    std::partial_ordering operator <=> (const ThreadSafeJson& other) const;
    bool operator == (const ThreadSafeJson& other) const;
    bool operator != (const ThreadSafeJson& other) const;
    nlohmann::json json() const;
    template <class T>
    T get() const {
        std::scoped_lock lock{ mutex_ };
        return j_.get<T>();
    }
private:
    nlohmann::json j_;
    mutable FastMutex mutex_;
};

void from_json(const nlohmann::json& j, ThreadSafeJson& t);
void to_json(nlohmann::json& j, const ThreadSafeJson& t);

}
