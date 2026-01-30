// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Frame_Time.hpp"
#include <cmath>
#include <stdexcept>

using namespace VA;

FrameTime::FrameTime(
    std::chrono::steady_clock::duration delay,
    std::function<std::chrono::steady_clock::duration()> dt,
    float alpha)
    : delay_{ delay }
    , dt_{ std::move(dt) }
    , alpha_{ alpha }
{}

FrameTime::~FrameTime() = default;

void FrameTime::tick() {
    if (smooth_time_ == std::chrono::steady_clock::time_point()) {
        smooth_time_ = std::chrono::steady_clock::now();
    } else {
        smooth_time_ += dt_();
        smooth_time_ -= std::chrono::steady_clock::duration{
            (std::chrono::steady_clock::rep)
            std::round(double(alpha_) * double((smooth_time_ - std::chrono::steady_clock::now()).count())) };
        // This is now replaced with "SetFps::completed_time()".
        // smooth_time_ = std::min(smooth_time_, std::chrono::steady_clock::now());
    }
}

void FrameTime::reset() {
    smooth_time_ = std::chrono::steady_clock::time_point();
}

bool FrameTime::is_up_to_date() const {
    return true;
}

std::chrono::steady_clock::time_point FrameTime::frame_time() const {
    if (smooth_time_ == std::chrono::steady_clock::time_point()) {
        throw std::runtime_error("FrameTime::frame_time on uninitialized object");
    }
    return smooth_time_ - delay_;
}
