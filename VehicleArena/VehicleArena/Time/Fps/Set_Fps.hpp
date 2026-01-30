// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <atomic>
#include <chrono>
#include <functional>
#include <list>
#include <mutex>

namespace VA {

class ISleeper;

class SetFps {
public:
    explicit SetFps(
        ISleeper* sleeper,
        std::function<std::chrono::steady_clock::time_point()> simulated_time = std::function<std::chrono::steady_clock::time_point()>(),
        std::function<bool()> paused = std::function<bool()>(),
        std::function<void()> on_tick = std::function<void()>());
    ~SetFps();
    void tick(std::chrono::steady_clock::time_point completed_time);
    void execute_oldest_funcs();
    std::chrono::steady_clock::time_point completed_time() const;
    std::chrono::steady_clock::time_point simulated_time() const;
    bool paused() const;
    void execute(const std::function<void()>& func);
    void request_stop();
    bool is_up_to_date() const;
private:
    bool execute_oldest_func();
    std::list<std::function<void()>> funcs_;
    std::atomic_bool stop_requested_;
    std::atomic<std::chrono::steady_clock::time_point> completed_time_;
    std::function<std::chrono::steady_clock::time_point()> simulated_time_;
    std::function<bool()> paused_;
    std::function<void()> on_tick_;
    std::mutex execute_mutex_;
    ISleeper* sleeper_;
};

}
