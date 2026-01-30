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

template <class TData, size_t tndim>
class PolygonEdgeException : public std::runtime_error {
public:
    PolygonEdgeException (const FixedArray<TData, tndim, 3>& poly, size_t i0, size_t i1, const std::string& what)
        : std::runtime_error{ what }
        , poly{ poly }
        , i0{ i0 }
        , i1{ i1 }
    {}
    std::string str(const std::string& message, const TransformationMatrix<double, double, 3>* m) const {
        std::stringstream sstr;
        sstr.precision(15);
        sstr << message << " at edge " <<
            i0 << ", " <<
            i1 << " of polygon ";
        for (size_t i = 0; i < tndim; ++i) {
            if (i != 0) {
                sstr << " <-> ";
            }
            sstr << poly[i];
        }
        if (m != nullptr) {
            sstr <<
            " | ";
            for (size_t i = 0; i < tndim; ++i) {
                if (i != 0) {
                    sstr << " <-> ";
                }
                sstr << m->transform(poly[i].template casted<double>());
            }
        }
        sstr << ": " << what();
        return sstr.str();
    }
    FixedArray<TData, tndim, 3> poly;
    size_t i0;
    size_t i1;
};

}
