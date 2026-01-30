// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Set_Fps.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Time/Fps/ISleeper.hpp>
#include <VehicleArena/Time/Sleep.hpp>
#include <stdexcept>
#include <thread>

using namespace VA;

SetFps::SetFps(
    ISleeper* sleeper,
    std::function<std::chrono::steady_clock::time_point()> simulated_time,
    std::function<bool()> paused,
    std::function<void()> on_tick)
    : stop_requested_{ false }
    , completed_time_()
    , simulated_time_{ std::move(simulated_time) }
    , paused_{ std::move(paused) }
    , on_tick_{ std::move(on_tick) }
    , sleeper_{ sleeper }
{}

SetFps::~SetFps() = default;

void SetFps::tick(std::chrono::steady_clock::time_point completed_time)
{
    completed_time_ = completed_time;
    if (sleeper_ != nullptr) {
        sleeper_->tick();
    }
    execute_oldest_funcs();
    if (paused() && !stop_requested_) {
        while (paused() && !stop_requested_) {
            if (on_tick_) {
                on_tick_();
            }
            while (execute_oldest_func());
            VA::sleep_for(std::chrono::microseconds(100));
        }
        if (sleeper_ != nullptr) {
            sleeper_->reset();
        }
    }
    if (on_tick_) {
        on_tick_();
    }
}

void SetFps::execute_oldest_funcs() {
    while (execute_oldest_func());
}

bool SetFps::execute_oldest_func() {
    std::function<void()> func;
    {
        std::scoped_lock lock{execute_mutex_};
        if (funcs_.empty()) {
            return false;
        }
        func = funcs_.front();
        funcs_.pop_front();
    }
    func();
    return true;
}

std::chrono::steady_clock::time_point SetFps::completed_time() const {
    return completed_time_;
}

std::chrono::steady_clock::time_point SetFps::simulated_time() const {
    if (!simulated_time_) {
        throw std::runtime_error("SetFps::simulated_time called but not set");
    }
    return simulated_time_();
}

bool SetFps::paused() const {
    if (!paused_) {
        throw std::runtime_error("SetFps::paused called but not set");
    }
    return paused_();
}

void SetFps::execute(const std::function<void()>& func) {
    std::scoped_lock lock{execute_mutex_};
    funcs_.push_back(func);
}

void SetFps::request_stop() {
    stop_requested_ = true;
}

bool SetFps::is_up_to_date() const {
    return (sleeper_ == nullptr) || sleeper_->is_up_to_date() || paused();
}
