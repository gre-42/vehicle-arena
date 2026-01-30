// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Realtime_Threads.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <mutex>
#include <stdexcept>
#include <thread>
#include <unordered_map>

#ifdef __ANDROID__
#include <sys/syscall.h>
#include <unistd.h>

static void pin_current_thread_to_cpu_range(uint32_t min, uint32_t max) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    for (auto i = min; i < max; ++i) {
        CPU_SET(i, &cpuset);
    }

    if (int res = sched_setaffinity(gettid(), sizeof(cpuset), &cpuset); res != 0)
    {
        switch (errno) {
            case EFAULT: throw std::runtime_error("EFAULT: A supplied memory address was invalid.");
            case EINVAL: throw std::runtime_error("EINVAL: Affinity bit mask contains no processors currently physically on the system and permitted to the process according to any restrictions that may be imposed by the \"cpuset\" mechanism");
            case ESRCH: throw std::runtime_error("ESRCH: The process whose ID is pid could not be found");
            default: VA::verbose_abort("Unknown errno: " + std::to_string(errno));
        }
    }
}
#elif defined(__linux__)
static void pin_current_thread_to_cpu_range(uint32_t min, uint32_t max) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    for (auto i = min; i < max; ++i) {
        CPU_SET(i, &cpuset);
    }
    if (int rc = pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset); rc != 0) {
        switch (rc) {
            case EFAULT: throw std::runtime_error("EFAULT: A supplied memory address was invalid.");
            case EINVAL: throw std::runtime_error("EINVAL: cpuset specified a CPU that was outside the set supported by the kernel");
            case ESRCH: throw std::runtime_error("No thread with the ID thread could be found");
            default: throw std::runtime_error("Unknown error calling pthread_setaffinity_np: " + std::to_string(rc));
        }
    }
}
#endif

#ifdef __linux__
#include <pthread.h>
static std::mutex mutex_;
uint32_t nreserved_realtime_threads_ = UINT32_MAX;
std::unordered_map<uint32_t, pthread_t> cpu_2_thread_;

void VA::register_realtime_thread() {
    std::scoped_lock lock{mutex_};
    if (nreserved_realtime_threads_ == UINT32_MAX) {
        throw std::runtime_error("Number of realtime-threads not set");
    }
    if (cpu_2_thread_.size() >= nreserved_realtime_threads_) {
        throw std::runtime_error("Not enough real-time threads reserved");
    }
    for (uint32_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
        if (!cpu_2_thread_.contains(i)) {
            pin_current_thread_to_cpu_range(i, i + 1);
            if (!cpu_2_thread_.try_emplace(i, pthread_self()).second) {
                verbose_abort("Internal error: Could not register real-time thread (1)");
            }
            return;
        }
    }
    verbose_abort("Internal error: Could not register real-time thread (2)");
}

void VA::unregister_realtime_thread() {
    std::scoped_lock lock{mutex_};
    for (const auto& [c, t] : cpu_2_thread_) {
        if (t == pthread_self()) {
            cpu_2_thread_.erase(c);
            return;
        }
    }
    verbose_abort("Could not unregister real-time thread");
}
#endif

#ifdef _WIN32
#include <Windows.h>
static std::mutex mutex_;
uint32_t nreserved_realtime_threads_ = UINT32_MAX;
std::unordered_map<uint32_t, HANDLE> cpu_2_thread_;

// From: https://stackoverflow.com/questions/1387064
static //Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }

    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);

    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

static void pin_current_thread_to_cpu_range(uint32_t min, uint32_t max) {
    DWORD_PTR affinity_mask = 0;
    for (uint32_t i = min; i < max; ++i) {
        affinity_mask |= (DWORD_PTR(1) << i);
    }
    if (DWORD_PTR rc = SetThreadAffinityMask(GetCurrentThread(), affinity_mask); rc == 0) {
        throw std::runtime_error("Could not set thread affinity mask: " + GetLastErrorAsString());
    }
}

void VA::register_realtime_thread() {
    std::scoped_lock lock{ mutex_ };
    if (nreserved_realtime_threads_ == UINT32_MAX) {
        throw std::runtime_error("Number of realtime-threads not set");
    }
    if (cpu_2_thread_.size() >= nreserved_realtime_threads_) {
        throw std::runtime_error("Not enough real-time threads reserved");
    }
    for (unsigned int i = 0; i < std::thread::hardware_concurrency(); ++i) {
        if (!cpu_2_thread_.contains(i)) {
            pin_current_thread_to_cpu_range(i, i + 1);
            if (!cpu_2_thread_.try_emplace(i, GetCurrentThread()).second) {
                verbose_abort("Internal error: Could not register real-time thread (1)");
            }
            return;
        }
    }
    verbose_abort("Internal error: Could not register real-time thread (2)");
}

void VA::unregister_realtime_thread() {
    std::scoped_lock lock{ mutex_ };
    for (const auto& [c, t] : cpu_2_thread_) {
        if (t == GetCurrentThread()) {
            cpu_2_thread_.erase(c);
            return;
        }
    }
    verbose_abort("Could not unregister real-time thread");
}

#endif

void VA::reserve_realtime_threads(uint32_t nreserved_realtime_threads) {
    std::scoped_lock lock{ mutex_ };
    if (nreserved_realtime_threads_ != UINT32_MAX) {
        throw std::runtime_error("Number of realtime-threads already set");
    }
    if (nreserved_realtime_threads == UINT32_MAX) {
        throw std::runtime_error("Invalid argument for nrealtime_threads");
    }
    if (std::thread::hardware_concurrency() < nreserved_realtime_threads) {
        throw std::runtime_error("Not enough CPUs available for number of real-time threads");
    }
    nreserved_realtime_threads_ = nreserved_realtime_threads;
}

void VA::unreserve_realtime_threads() {
    std::scoped_lock lock{ mutex_ };
    if (nreserved_realtime_threads_ == UINT32_MAX) {
        verbose_abort("Number of realtime-threads not set");
    }
    if (!cpu_2_thread_.empty()) {
        verbose_abort("CPU-to-thread mapping not empty");
    }
    nreserved_realtime_threads_ = UINT32_MAX;
}

void VA::pin_background_thread() {
    std::scoped_lock lock{ mutex_ };
    if (nreserved_realtime_threads_ == UINT32_MAX) {
        throw std::runtime_error("Number of realtime-threads not set");
    }
    pin_current_thread_to_cpu_range(nreserved_realtime_threads_, std::thread::hardware_concurrency());
}
