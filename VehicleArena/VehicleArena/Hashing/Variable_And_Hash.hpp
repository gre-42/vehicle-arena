// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Hash.hpp>
#include <VehicleArena/Hashing/Std_Hash.hpp>
#include <compare>

namespace VA {

template <class T>
class VariableAndHash {
public:
    VariableAndHash& operator = (const T& other) {
        *this = VariableAndHash<T>(other);
        return *this;
    }
    VariableAndHash& operator = (T&& other) {
        *this = VariableAndHash<T>(std::move(other));
        return *this;
    }
    VariableAndHash()
        : variable_()
        , hash_{ hash_combine(variable_) }
    {}
    explicit VariableAndHash(const T& v)
        : variable_{ v }
        , hash_{ hash_combine(variable_) }
    {}
    explicit VariableAndHash(T&& v)
        : variable_{ std::move(v) }
        , hash_{ hash_combine(variable_) }
    {}
    VariableAndHash(const VariableAndHash<const T>& other)
        : variable_{ other.variable_ }
        , hash_{ other.hash_ }
    {}
    VariableAndHash(VariableAndHash<const T>&& other)
        : variable_{ std::move(other.variable_) }
        , hash_{ other.hash_ }
    {}
    template <class Arg>
        requires std::is_convertible_v<Arg, const T&>
    explicit VariableAndHash(Arg&& value)
        : variable_{ std::forward<Arg>(value) }
        , hash_{ hash_combine(variable_) }
    {}
    const T& operator * () const {
        return variable_;
    }
    const T* operator -> () const {
        return &variable_;
    }
    // operator const T&() const {
    //     return variable_;
    // }
    std::strong_ordering operator <=> (const VariableAndHash& other) const {
        return hash_ <=> other.hash_;
    }
    bool operator == (const VariableAndHash& other) const {
        return hash_ == other.hash_;
    }
    size_t hash() const {
        return hash_;
    }
    template <class Archive>
    void serialize(Archive& archive) {
        archive(variable_);
        archive(hash_);
    }
private:
    T variable_;
    size_t hash_;
};

}

template <class T>
struct std::hash<VA::VariableAndHash<T>>
{
    std::size_t operator() (const VA::VariableAndHash<T>& k) const {
        return k.hash();
    }
};
