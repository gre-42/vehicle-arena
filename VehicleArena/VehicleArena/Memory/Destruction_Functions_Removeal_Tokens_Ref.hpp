// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Std_Hash.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Memory/Destruction_Functions.hpp>
#include <compare>
#include <type_traits>

namespace VA {

template <class T>
class DestructionFunctionsTokensRef {
    DestructionFunctionsTokensRef(const DestructionFunctionsTokensRef&) = delete;
    DestructionFunctionsTokensRef& operator = (const DestructionFunctionsTokensRef&) = delete;
public:
    using value_type = T;

    template <class TDerived>
        requires std::is_convertible_v<TDerived&, T&>
    inline DestructionFunctionsTokensRef(
        const DanglingBaseClassRef<TDerived>& o,
        DestructionFunctions& on_destroy,
        SourceLocation loc)
        : object_{ o }
        , on_object_destroy_{ on_destroy, loc }
    {}
    template <class TDerived>
        requires std::is_convertible_v<TDerived&, T&>
    inline DestructionFunctionsTokensRef(const DanglingBaseClassRef<TDerived>& o, SourceLocation loc)
        : DestructionFunctionsTokensRef{ o, o->on_destroy, loc }
    {}
    inline T* operator -> () const {
        return &object_.get();
    }
    inline T& get() const {
        return object_.get();
    }
    inline const DanglingBaseClassRef<T>& object() const {
        return object_;
    }
    inline void on_destroy(std::function<void()> func, SourceLocation loc) const {
        on_object_destroy_.add(std::move(func), std::move(loc));
    }
    inline std::strong_ordering operator <=> (const DanglingBaseClassPtr<T>& other) const {
        return &object_.get() <=> other.get();
    }
    inline bool operator == (const DanglingBaseClassPtr<T>& other) const {
        return &object_.get() == other.get();
    }
    inline bool operator != (const DanglingBaseClassPtr<T>& other) const {
        return &object_.get() != other.get();
    }
private:
    DanglingBaseClassRef<T> object_;
    mutable DestructionFunctionsRemovalTokens on_object_destroy_;
};

}
