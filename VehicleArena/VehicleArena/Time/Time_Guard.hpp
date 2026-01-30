// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <chrono>
#include <compare>
#include <iosfwd>
#include <map>
#include <string>
#include <thread>
#include <vector>

namespace VA {

template <class TSize>
class Svg;

struct TimeEvent {
    size_t event_id;
    std::chrono::steady_clock::time_point time;
    const char* message = nullptr;
    size_t stack_size = SIZE_MAX;
    std::strong_ordering operator <=> (const TimeEvent& other) const {
        return event_id <=> other.event_id;
    }
};

struct CalledFunction {
    std::chrono::steady_clock::time_point start_time;
    std::chrono::steady_clock::time_point end_time;
    const char* message = nullptr;
    std::string group;
    size_t stack_size = SIZE_MAX;
};

struct ThreadTimeInfo {
    std::vector<TimeEvent> events;
    std::vector<CalledFunction> called_functions;
    size_t stack_size = 0;
    size_t event_id = 0;
    size_t called_function_id = 0;
};

enum class MaxLogLengthExceededBehavior {
    THROW_EXCEPTION,
    PERIODIC
};

class TimeGuard {
    TimeGuard(const TimeGuard&) = delete;
    TimeGuard& operator = (const TimeGuard&) = delete;
public:
    TimeGuard(const char* message, const std::string& group);
    ~TimeGuard();
    static void initialize(
        size_t max_log_length,
        MaxLogLengthExceededBehavior max_log_length_exceeded_behavior);
    static void write_svg(const std::thread::id& tid, const std::string& filename);
    static void print_groups(std::ostream& ostr);
    static bool is_empty(const std::thread::id& tid);
private:
    static void insert_event(const TimeEvent& e);
    static void insert_called_function(const CalledFunction& e);
    static std::chrono::steady_clock::time_point init_time_;
    static std::map<std::thread::id, ThreadTimeInfo> thread_time_infos_;
    static size_t max_log_length_;
    static MaxLogLengthExceededBehavior max_log_length_exceeded_behavior_;
    CalledFunction called_function_;
};

}
