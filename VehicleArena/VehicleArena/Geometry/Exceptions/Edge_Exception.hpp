// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <sstream>
#include <stdexcept>
#include <string>

namespace p2t {

struct Point;

}

namespace Mlib {

template <class TPos>
class EdgeException: public std::runtime_error {
public:
    EdgeException(const FixedArray<TPos, 3>& a, const FixedArray<TPos, 3>& b, const std::string& what)
        : std::runtime_error{ what }
        , a{ a }
        , b{ b }
    {}
    EdgeException(const FixedArray<TPos, 2>& a, const FixedArray<TPos, 2>& b, const std::string& what)
        : std::runtime_error{ what }
        , a{ a(0), a(1), (TPos)0.f }
        , b{ b(0), b(1), (TPos)0.f }
    {}
    EdgeException(const p2t::Point* a, const p2t::Point* b, const std::string& what)
        : std::runtime_error{ what }
        , a{ (TPos)((double*)a)[0], (TPos)((double*)a)[1], (TPos)0 }
        , b{ (TPos)((double*)b)[0], (TPos)((double*)b)[1], (TPos)0 }
    {}
    std::string str(const std::string& message, const TransformationMatrix<double, double, 3>* m) const {
        std::stringstream sstr;
        sstr.precision(15);
        sstr << message << " at edge " <<
            a <<
            " -> " <<
            b;
        if (m != nullptr) {
            sstr <<
                " | " <<
                m->transform(a.template casted<double>()) <<
                " -> " <<
                m->transform(b.template casted<double>());
        }
        sstr << ": " << what();
        return sstr.str();
    }
    FixedArray<TPos, 3> a;
    FixedArray<TPos, 3> b;
};

template <class TPos, size_t ndim>
inline EdgeException<TPos> edge_exception(const FixedArray<TPos, ndim>& a, const FixedArray<TPos, ndim>& b, const std::string& what) {
    return EdgeException<TPos>{a, b, what};
}

inline EdgeException<double> edge_exception(const p2t::Point* a, const p2t::Point* b, const std::string& what) {
    return EdgeException<double>{a, b, what};
}

}
