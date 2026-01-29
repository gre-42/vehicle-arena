// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Stats/T_Distribution.hpp>
#include <Mlib/Throw_Or_Abort.hpp>

namespace Mlib {

/**
 * Iterator yielding mean and variance.
 * http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
 */
template <class TData>
class MeanVarianceIterator{
public:
    void add(const TData& x) {
        ++n_;
        TData delta = x - mean_;
        mean_ += delta / (TData)n_;
        M2_ += delta * (x - mean_);
    }
    TData mean() const {
        return mean_;
    }
    TData var() const {
        if (n_ < 2) {
            THROW_OR_ABORT("n < 2, can not compute variance");
        }
        return M2_ / TData(n_ - 1);
    }
    TData cohens_d(const TData& epsilon = 0) const {
        return mean() / (std::sqrt(var()) + epsilon);
    }
    TData t(const TData& epsilon = 0) const {
        return mean() / (std::sqrt(var() / TData(n_)) + epsilon);
    }
    TData p1() const {
        if (var() < 1e-12) {
            return 0;
        } else {
            return student_t_sf<TData>(t(), TData(n_ - 1));
        }
    }
    TData p2() const {
        if (var() < 1e-12) {
            return 0;
        } else {
            return 2 * student_t_sf<TData>(std::abs(t()), TData(n_ - 1));
        }
    }
    size_t n() const {
        return n_;
    }
private:
    size_t n_ = 0;
    TData mean_ = 0;
    TData M2_ = 0;
};

}
