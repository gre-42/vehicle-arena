// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Usage_Counter.hpp"
#include <mutex>

using namespace VA;

CounterUser::CounterUser(const DanglingBaseClassRef<UsageCounter>& counter)
    : usage_counter_{ counter }
    , value_{ false }
{}

CounterUser::~CounterUser() {
    set(false);
}

void CounterUser::set(bool value) {
    std::scoped_lock lock{ mutex_ };
    if (value && !value_) {
        usage_counter_->increase();
    }
    if (!value && value_) {
        usage_counter_->decrease();
    }
    value_ = value;
}

UsageCounter::UsageCounter()
    : count_{ 0 }
{}

UsageCounter::~UsageCounter() = default;

void UsageCounter::increase() {
    ++count_;
}

void UsageCounter::decrease() {
    --count_;
}

size_t UsageCounter::count() const {
    return count_;
}
