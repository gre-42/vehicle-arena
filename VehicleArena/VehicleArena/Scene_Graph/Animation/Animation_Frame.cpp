// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Animation_Frame.hpp"
#include <algorithm>
#include <cmath>
#include <shared_mutex>
#include <stdexcept>

namespace VA {

void AnimationFrame::advance_time(float dt, AnimationWrapMode wrap_mode) {
    if (!std::isnan(time)) {
        if (std::isnan(begin) != std::isnan(end)) {
            throw std::runtime_error("Inconsistent begin and end NaN-ness (0)");
        }
        if (!std::isnan(begin)) {
            if (end < begin) {
                throw std::runtime_error("Loop end before loop begin");
            }
            if (time < begin) {
                throw std::runtime_error("Loop time before loop begin");
            }
            if (time > end) {
                throw std::runtime_error("Loop time after loop end");
            }
            if (end == begin) {
                time = begin;
            } else {
                if (wrap_mode == AnimationWrapMode::PERIODIC) {
                    time = std::clamp(
                        begin + std::fmod(
                            time + dt - begin,
                            end - begin),
                        begin,
                        end);
                } else {
                    time = std::min(time + dt, end);
                }
            }
        }
    }
}

bool AnimationFrame::is_nan() const {
    if (std::isnan(begin) != std::isnan(end)) {
        throw std::runtime_error("Inconsistent begin and end NaN-ness (1)");
    }
    if (std::isnan(time) != std::isnan(end)) {
        throw std::runtime_error("Inconsistent begin and end NaN-ness (2)");
    }
    return std::isnan(time);
}

float PeriodicAnimationFrame::time() const {
    std::shared_lock lock{ *mutex_ };
    return frame_.time;
}

void PeriodicAnimationFrame::advance_time(float dt) {
    std::lock_guard lock{ *mutex_ };
    frame_.advance_time(dt, AnimationWrapMode::PERIODIC);
}

float AperiodicAnimationFrame::time() const {
    std::shared_lock lock{ *mutex_ };
    return frame_.time;
}

void AperiodicAnimationFrame::advance_time(float dt) {
    std::lock_guard lock{ *mutex_ };
    frame_.advance_time(dt, AnimationWrapMode::APERIODIC);
}

bool AperiodicAnimationFrame::active() const {
    if (std::isnan(frame_.begin) != std::isnan(frame_.end)) {
        throw std::runtime_error("Inconsistent begin and end NaN-ness (3)");
    }
    return !frame_.is_nan() && (frame_.time != frame_.end);
}

bool AperiodicAnimationFrame::ran_to_completion() const {
    return !frame_.is_nan() && (frame_.time == frame_.end);
}

float AperiodicAnimationFrame::duration() const {
    return frame_.end - frame_.begin;
}

float AperiodicAnimationFrame::elapsed() const {
    return frame_.time - frame_.begin;
}

bool AperiodicAnimationFrame::is_nan() const {
    return frame_.is_nan();
}

}
