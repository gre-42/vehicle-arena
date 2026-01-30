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

namespace Mlib {

template <class TData>
class TriangleException : public std::runtime_error {
public:
    TriangleException(const FixedArray<TData, 2>& a, const FixedArray<TData, 2>& b, const FixedArray<TData, 2>& c, const std::string& what)
        : std::runtime_error{ what }
        , a{ FixedArray<TData, 3>{a(0), a(1), TData(0.f)} }
        , b{ FixedArray<TData, 3>{b(0), b(1), TData(0.f)} }
        , c{ FixedArray<TData, 3>{c(0), c(1), TData(0.f)} }
    {}
    TriangleException(const FixedArray<TData, 3>& a, const FixedArray<TData, 3>& b, const FixedArray<TData, 3>& c, const std::string& what)
        : std::runtime_error{ what }
        , a{ a }
        , b{ b }
        , c{ c }
    {}
    std::string str(const std::string& message, const TransformationMatrix<double, double, 3>* m) const {
        std::stringstream sstr;
        sstr.precision(15);
        sstr << message << " at triangle " <<
            a <<
            " <-> " <<
            b <<
            " <-> " <<
            c;
        if (m != nullptr) {
            sstr <<
                " | " <<
                m->transform(a.template casted<double>()) <<
                " <-> " <<
                m->transform(b.template casted<double>()) <<
                " <-> " <<
                m->transform(c.template casted<double>());
        }
        sstr << ": " << what();
        return sstr.str();
    }
    FixedArray<TData, 3> a;
    FixedArray<TData, 3> b;
    FixedArray<TData, 3> c;
};

}
