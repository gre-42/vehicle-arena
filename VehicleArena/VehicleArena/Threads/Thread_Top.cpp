// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Thread_Top.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <VehicleArena/Threads/Get_Thread_Name.hpp>
#include <list>
#include <map>
#include <mutex>
#include <sstream>

using namespace VA;

static FastMutex mutex;
static std::map<ThreadIdentifier, std::list<std::string>> tasks;

FunctionGuard::FunctionGuard(std::string task_name)
    : id{ get_thread_name(), std::this_thread::get_id() }
{
    std::scoped_lock lock{ mutex };
    task_name_ = &tasks[id].emplace_back(std::move(task_name));
}

FunctionGuard::~FunctionGuard()
{
    std::scoped_lock lock{ mutex };
    auto it = tasks.find(id);
    if (it == tasks.end()) {
        verbose_abort("Could not find task");
    }
    it->second.pop_back();
    if (it->second.empty()) {
        tasks.erase(it);
    }
}

void FunctionGuard::update(std::string task_name) {
    std::scoped_lock lock{ mutex };
    *task_name_ = std::move(task_name);
}

std::string VA::thread_top() {
    std::stringstream sstr;
    std::scoped_lock lock{ mutex };
    for (const auto& [id, ts] : tasks) {
        sstr << "Thread name: " << id.name << ", ID: " << id.id << '\n';
        for (const auto& t : ts) {
            if (!t.empty()) {
                sstr << "    " << t << '\n';
            }
        }
    }
    return sstr.str();
}
