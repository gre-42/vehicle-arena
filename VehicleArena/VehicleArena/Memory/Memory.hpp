// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Destruction_Observer.hpp>

namespace VA {

template <class T, class TSender>
class observer_ptr {
public:
    template <class TDerived>
    observer_ptr(const DanglingBaseClassPtr<TDerived>& ptr)
        : ptr_{ ptr }
        , observer_{ ptr }
    {}
    template <class TDerived>
    observer_ptr(TDerived& ptr, SourceLocation loc)
        : ptr_{ ptr, loc }
        , observer_{ ptr, loc }
    {}
    observer_ptr(const DanglingBaseClassPtr<T>& ptr, const DanglingBaseClassPtr<DestructionObserver<TSender>>& observer)
        : ptr_{ ptr }
        , observer_{ observer }
    {}
    DanglingBaseClassPtr<T> get() const {
        return ptr_;
    }
    DanglingBaseClassPtr<DestructionObserver<TSender>> observer() const {
        return observer_;
    }
private:
    DanglingBaseClassPtr<T> ptr_;
    DanglingBaseClassPtr<DestructionObserver<TSender>> observer_;
};

}
