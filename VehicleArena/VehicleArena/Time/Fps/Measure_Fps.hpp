// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Time/Fps/ISleeper.hpp>
#include <chrono>
#include <climits>

namespace VA {

class MeasureFps: public ISleeper {
public:
    MeasureFps(double alpha = 0.005, unsigned int print_interval = UINT_MAX);
    ~MeasureFps();

    virtual void tick() override;
    virtual void reset() override;
    virtual bool is_up_to_date() const override;

    double mean_dt() const;

    double last_fps() const;
    double mean_fps() const;
    double mad_fps() const;
    double min_fps() const;
    double max_fps() const;
private:
    std::chrono::steady_clock::time_point last_time_;
    double last_dt_;
    double mean_dt_;
    double mad_dt_;
    double alpha_;
    unsigned int print_counter_;
    unsigned int print_interval_;
};

}
