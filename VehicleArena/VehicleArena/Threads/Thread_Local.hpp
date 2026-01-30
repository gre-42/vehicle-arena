// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Misc/Features.hpp>
#include <stdexcept>

#ifndef WITHOUT_THREAD_LOCAL

#define THREAD_LOCAL(T) thread_local T

#else

#define THREAD_LOCAL(T) ::VA::ThreadLocal<T>

#include <VehicleArena/Os/Os.hpp>
#include <pthread.h>
#include <stdexcept>

namespace VA {

template <class T>
class ThreadLocal {
    ThreadLocal(const ThreadLocal&) = delete;
    ThreadLocal& operator = (const ThreadLocal&) = delete;
public:
    ThreadLocal(const T& initial_value)
    : initial_value_{initial_value}
    {
        if (pthread_key_create(&thread_specific_key_, data_destructor) != 0) {
            throw std::runtime_error("Cannot create thread-specific key");
        }
    }
    ~ThreadLocal() {
        if (pthread_key_delete(thread_specific_key_) != 0) {
            verbose_abort("Cannot delete thread-specific key");
        }
    }
    T& operator = (const T& value) {
        auto& v = get();
        v = value;
        return v;
    }
    auto& operator * () {
        return *get();
    }
    T& operator -> () {
        return get();
    }
    operator T& () {
        return get();
    }
    T& get() {
        auto* res = (T*)pthread_getspecific(thread_specific_key_);
        if (res == nullptr) {
            res = new T{initial_value_};
            if (pthread_setspecific(thread_specific_key_, res) != 0) {
                delete res;
                throw std::runtime_error("Cannot set thread-specific value");
            }
        }
        return *res;
    }
private:
    static void data_destructor(void* data) {
        delete (T*)data;
    }
    pthread_key_t thread_specific_key_;
    T initial_value_;
};

}

#endif
