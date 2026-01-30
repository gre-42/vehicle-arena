// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Dispatcher.hpp"

using namespace VA;

Dispatcher::Dispatcher(std::chrono::milliseconds wait_time)
    : nparticipants_{ 0 }
    , nwaiting_{ 0 }
    , nproduced_{ 0 }
    , wait_time_{ wait_time }
{}

Dispatcher::~Dispatcher() = default;

void Dispatcher::produce() {
    {
        std::unique_lock lock(mutex_);
        cv_.wait_for(lock, wait_time_, [this] { return (nproduced_ == 0) && (nwaiting_ == nparticipants_); });
        nparticipants_ = nwaiting_;
        nproduced_ = nparticipants_;
    }
    cv_.notify_all();
}

void Dispatcher::wait_for_data() {
    ++nwaiting_;
    cv_.notify_all();
    {
        std::unique_lock lock(mutex_);
        cv_.wait_for(lock, wait_time_, [this] { return nproduced_ > 0; });
    }
    --nwaiting_;
}

void Dispatcher::consume() {
    bool notify = false;
    {
        std::scoped_lock lock(mutex_);
        if (nproduced_ > 0) {
            --nproduced_;
            if (nproduced_ == 0) {
                notify = true;
            }
        }
    }
    if (notify) {
        cv_.notify_all();
    }
}
