// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <atomic>
#include <iostream>

namespace VA {

template <class TData>
class resource_ptr;

template <class TData>
class resource_ptr_target {
    friend resource_ptr<TData>;
public:
    explicit resource_ptr_target(TData* data)
    : data_{data}
    {}
    ~resource_ptr_target() {
        if (ref_count_ != 0) {
            lerr() << "WARNING: resource_ptr_target destructor with nonzero refcount";
        } else {
            delete data_;
        }
    }
    resource_ptr<TData> instantiate() {
        return resource_ptr<TData>{this};
    }
private:
    TData* data_;
    std::atomic_size_t ref_count_ = 0;
};

template <class TData>
class resource_ptr {
public:
    resource_ptr() = delete;
    resource_ptr(const resource_ptr&) = delete;
    resource_ptr& operator = (const resource_ptr&) = delete;
    resource_ptr(resource_ptr_target<TData>* target)
    : target_{target}
    {
        ++target_->ref_count_;
    }
    ~resource_ptr() {
        --target_->ref_count_;
    }
    TData& operator * () {
        return *target_->data_;
    }
    TData* operator -> () {
        return target_->data_;
    }
private:
    resource_ptr_target<TData>* target_;
};

}
