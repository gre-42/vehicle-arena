// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Stats/Clamp_Uniform.hpp>
#include <cstddef>
#include <vector>

namespace Mlib {

template <class TRationalSequence, class TFloat>
class FloatSequence {
public:
    using value_type = TFloat;

    FloatSequence(const TRationalSequence& h)
        : h_{ h }
    {}
    TFloat operator () () {
        return h_().template as_float<TFloat>();
    }
private:
    TRationalSequence h_;
};

template <class TSourceSequence>
class UniformSequence {
public:
    using value_type = typename std::remove_reference_t<TSourceSequence>::value_type;

    UniformSequence(const TSourceSequence& s, const value_type& low = 0, const value_type& high = 1)
        : s_{ s }
        , low_{ low }
        , high_{ high }
    {}
    value_type operator () () {
        return clamp_uniform(
            low_ + s_() * (high_ - low_),
            low_,
            high_);
    }
private:
    TSourceSequence s_;
    value_type low_;
    value_type high_;
};

template <class TSourceSequence>
class BlockPermutedSequence {
public:
    using value_type = typename std::remove_reference_t<TSourceSequence>::value_type;

    BlockPermutedSequence(const TSourceSequence& s, unsigned int seed, size_t buffer_size)
        : s_{ s }
        , irng_{ seed, 0, buffer_size - 1 }
        , buffer_(buffer_size)
    {
        if (buffer_size == 0) {
            THROW_OR_ABORT("Buffer size is zero");
        }
        this->seed(seed);
    }
    value_type operator () () {
        size_t i = irng_();
        auto result = buffer_[i];
        buffer_[i] = s_();
        return result;
    }
    void seed(unsigned int seed) {
        irng_.seed(seed);
        for (auto& v : buffer_) {
            v = s_();
        }
    }
private:
    TSourceSequence s_;
    FastUniformIntRandomNumberGenerator<size_t> irng_;
    std::vector<value_type> buffer_;
};

}
