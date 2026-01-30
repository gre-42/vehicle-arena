// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Background_Loop.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Threads/Thread_Affinity.hpp>
#include <VehicleArena/Threads/Thread_Initializer.hpp>
#include <VehicleArena/Time/Sleep.hpp>
#include <stdexcept>

using namespace VA;

BackgroundLoop::BackgroundLoop(std::string thread_name)
    : i_{ SIZE_MAX }
    , done_{ true }
    , thread_{ [this, tn = std::move(thread_name)](){
        try {
            ThreadInitializer ti{ tn, ThreadAffinity::POOL };
            while (true) {
                {
                    std::unique_lock lck{ mutex_ };
                    task_ready_cv_.wait(lck, [this]() { return !done_ || thread_.get_stop_token().stop_requested(); });
                    if (thread_.get_stop_token().stop_requested()) {
                        task_ = std::function<void()>();
                        return;
                    }
                    if (!task_) {
                        verbose_abort("Task not set");
                    }
                }
                task_();
                {
                    std::scoped_lock lck{ mutex_ };
                    task_ = std::function<void()>();
                    done_ = true;
                }
                done_cv_.notify_one();
            }
        } catch (const std::exception& e) {
            verbose_abort("Unhandled exception in background-loop: " + std::string{e.what()});
        }
    } }
{}

BackgroundLoop::~BackgroundLoop() {
    shutdown();
}

void BackgroundLoop::shutdown() {
    if (thread_.get_stop_token().stop_requested()) {
        return;
    }
    thread_.request_stop();
    task_ready_cv_.notify_one();
    done_cv_.notify_all();
    thread_.join();
}

WorkerStatus BackgroundLoop::tick(size_t update_interval) {
    i_ = (i_ + 1) % update_interval;
    if (done_) {
        if (i_ == 0) {
            return WorkerStatus::IDLE;
        } else {
            return WorkerStatus::BUSY;
        }
    } else {
        return WorkerStatus::BUSY;
    }
}

void BackgroundLoop::run(const std::function<void()>& task) {
    if (!try_run(task)) {
        throw std::runtime_error("BackgroundLoop::run despite not done");
    }
}

bool BackgroundLoop::try_run(const std::function<void()>& task) {
    if (!thread_.joinable()) {
        throw std::runtime_error("BackgroundLoop::run despite not joinable thread");
    }
    {
        std::scoped_lock lck{ mutex_ };
        if (!done_) {
            return false;
        }
        task_ = task;
        done_ = false;
    }
    task_ready_cv_.notify_one();
    return true;
}

bool BackgroundLoop::done() const {
    std::scoped_lock lock{ mutex_ };
    return done_;
}

void BackgroundLoop::wait_until_done() const {
    std::unique_lock lck{ mutex_ };
    done_cv_.wait(lck, [this]() { return done_ || thread_.get_stop_token().stop_requested(); });
}

void BackgroundLoop::wait_until_done_and_run(
    const std::function<void()>& task)
{
    while (!thread_.get_stop_token().stop_requested() && !try_run(task)) {
        wait_until_done();
    }
}
