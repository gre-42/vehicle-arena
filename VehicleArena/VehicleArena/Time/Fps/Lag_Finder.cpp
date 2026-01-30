// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Lag_Finder.hpp"
#include <VehicleArena/Os/Os.hpp>

using namespace VA;

PeriodicLagFinder::PeriodicLagFinder(std::string prefix, const std::chrono::milliseconds& lag_duration)
: prefix_{ std::move(prefix) },
  lag_duration_{ lag_duration },
  end_time_{ std::chrono::steady_clock::now() }
{}

void PeriodicLagFinder::start() {
    start_time_ = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(start_time_ - end_time_);
    if (duration > lag_duration_) {
        linfo() << prefix_ << "start " << duration.count();
    }
}

void PeriodicLagFinder::stop() {
    end_time_ = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time_ - start_time_);
    if (duration > lag_duration_) {
        linfo() << prefix_ << "end " << duration.count();
    }
}

AperiodicLagFinder::AperiodicLagFinder(std::string prefix, const std::chrono::milliseconds& lag_duration)
    : prefix_{ std::move(prefix) }
    , lag_duration_{ lag_duration }
    , start_time_{ std::chrono::steady_clock::now() }
{}

AperiodicLagFinder::~AperiodicLagFinder() {
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time_);
    if (duration > lag_duration_) {
        linfo() << prefix_ << duration.count();
    }
}
