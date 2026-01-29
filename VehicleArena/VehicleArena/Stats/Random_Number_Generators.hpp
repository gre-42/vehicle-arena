// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Math/Float_Type.hpp>
#include <random>

namespace Mlib {

template <class TData>
class UniformRandomNumberGenerator {
public:
    typedef typename FloatType<TData>::value_type float_type;
    explicit UniformRandomNumberGenerator(unsigned int seed, const float_type& low = 0, const float_type& high = 1)
    : e_(seed),
      d_(low, high)
    {}
    TData operator () () {
        return d_(e_);
    }
    void seed(unsigned int seed) {
        e_.seed(seed);
    }
private:
    std::mt19937 e_;
    std::uniform_real_distribution<typename FloatType<TData>::value_type> d_;
};

template <class TData>
class UniformIntRandomNumberGenerator {
public:
    explicit UniformIntRandomNumberGenerator(unsigned int seed, const TData& low, const TData& high)
    : e_(seed),
      d_(low, high)
    {}
    TData operator () () {
        return d_(e_);
    }
    void seed(unsigned int seed) {
        e_.seed(seed);
    }
private:
    std::mt19937 e_;
    std::uniform_int_distribution<TData> d_;
};

template <class TData>
class NormalRandomNumberGenerator {
public:
    typedef typename FloatType<TData>::value_type float_type;
    explicit NormalRandomNumberGenerator(unsigned int seed, const float_type& mean = 0, const float_type& stddev = 1)
    : e_(seed),
      d_(mean, stddev)
    {}
    TData operator () () {
        return d_(e_);
    }
    void seed(unsigned int seed) {
        e_.seed(seed);
    }
private:
    std::mt19937 e_;
    std::normal_distribution<typename FloatType<TData>::value_type> d_;
};

template <class TData>
class GammaRandomNumberGenerator {
public:
    typedef typename FloatType<TData>::value_type float_type;
    explicit GammaRandomNumberGenerator(unsigned int seed, const float_type& alpha, const float_type& beta = 1)
    : e_(seed),
      d_(alpha, beta)
    {}
    TData operator () () {
        return d_(e_);
    }
    void seed(unsigned int seed) {
        e_.seed(seed);
    }
private:
    std::mt19937 e_;
    std::gamma_distribution<typename FloatType<TData>::value_type> d_;
};

}
