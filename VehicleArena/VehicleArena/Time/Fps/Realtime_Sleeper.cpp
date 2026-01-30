// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Realtime_Sleeper.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Time/Busy_Sleep_Until.hpp>
#include <cmath>
#include <thread>

using namespace VA;

RealtimeSleeper::RealtimeSleeper(
    std::string prefix,
    float dt,
    float max_residual_time,
    bool print_residual_time)
    : dt_{ dt }
    , max_residual_time_{ max_residual_time }
    , print_residual_time_{ print_residual_time }
    , sim_time_{ std::chrono::steady_clock::now() }
    , prefix_{ std::move(prefix) }
    , is_up_to_date_{ true }
{}

RealtimeSleeper::~RealtimeSleeper() = default;

void RealtimeSleeper::tick() {
    sim_time_ += std::chrono::duration_cast<std::chrono::steady_clock::duration>(std::chrono::duration<double>{double(dt_)});
    std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
    std::chrono::steady_clock::duration residual_time = sim_time_ - current_time;
    if (!std::isnan(max_residual_time_) && (std::chrono::duration<float>(residual_time).count() < -max_residual_time_)) {
        sim_time_ = current_time;
        if (print_residual_time_) {
            linfo() << prefix_ << "resetting sim time";
        }
        is_up_to_date_ = true;
    } else {
        if (residual_time.count() > 0) {
            is_up_to_date_ = true;
            // busy wait
            // while(residual_time.count() > 0) {
            //     current_time = std::chrono::steady_clock::now();
            //     residual_time = sim_time_ - current_time;
            // }
            // VA::sleep_for(residual_time);
            busy_sleep_until(sim_time_);
        } else {
            is_up_to_date_ = false;
            if (print_residual_time_) {
                linfo() <<
                    prefix_ <<
                    "residual time: " <<
                    std::chrono::duration_cast<std::chrono::milliseconds>(-residual_time).count() <<
                    " ms";
            }
        }
    }
}

void RealtimeSleeper::reset() {
    sim_time_ = std::chrono::steady_clock::now();
    if (print_residual_time_) {
        linfo() << prefix_ << "resetting sim time after pause";
    }
}

bool RealtimeSleeper::is_up_to_date() const {
    return is_up_to_date_;
}

void RealtimeSleeper::set_dt(float dt) {
    dt_ = dt;
}

std::chrono::steady_clock::time_point RealtimeSleeper::simulated_time() const {
    return sim_time_;
}
