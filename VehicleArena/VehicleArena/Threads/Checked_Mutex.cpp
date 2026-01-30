// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Checked_Mutex.hpp"
#include <stdexcept>
#include <stdexcept>

using namespace VA;

CheckedMutex::CheckedMutex() = default;

CheckedMutex::~CheckedMutex() = default;

void CheckedMutex::lock() {
    if (locked_by_caller()) {
        throw std::runtime_error("Mutex already locked by caller");
    }
    mutex_.lock();
    holder_ = std::this_thread::get_id(); 
}

void CheckedMutex::unlock() {
    holder_ = std::thread::id();
    mutex_.unlock();
}

void CheckedMutex::lock_shared() {
    mutex_.lock_shared();
}

void CheckedMutex::unlock_shared() {
    mutex_.unlock_shared();
}

bool CheckedMutex::locked_by_caller() const {
    return holder_ == std::this_thread::get_id();
}

void CheckedMutex::assert_locked_by_caller() const {
    if (!locked_by_caller()) {
        throw std::runtime_error("Mutex not locked by caller");
    }
}
