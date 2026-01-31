// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Barycentric_Coordinates.hpp"
#include <VehicleArena/Geometry/Coordinates/Normalized_Points_Fixed.hpp>
#include <VehicleArena/Geometry/Exceptions/Triangle_Exception.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <sstream>
#include <stdexcept>

using namespace VA;

/**
 * From: https://gamedev.stackexchange.com/a/23745/140709
 * Compute barycentric coordinates (u, v, w) for
 * point p with respect to triangle (a, b, c)
 */
void VA::barycentric(
    const FixedArray<double, 2>& p,
    const FixedArray<double, 2>& a,
    const FixedArray<double, 2>& b,
    const FixedArray<double, 2>& c,
    double &u,
    double &v,
    double &w)
{
    NormalizedPointsFixed<double> np{ ScaleMode::DIAGONAL, OffsetMode::CENTERED };
    np.add_point(a);
    np.add_point(b);
    np.add_point(c);
    TransformationMatrix<double, double, 2> m = np.normalization_matrix();

    FixedArray<double, 2> an = m.transform(a);
    FixedArray<double, 2> bn = m.transform(b);
    FixedArray<double, 2> cn = m.transform(c);

    FixedArray<double, 2> v0 = (bn - an);
    FixedArray<double, 2> v1 = (cn - an);
    double d00 = dot0d(v0, v0);
    double d01 = dot0d(v0, v1);
    double d11 = dot0d(v1, v1);
    double denom = d00 * d11 - d01 * d01;
    // FixedArray<TData, 2, 2> M = dot2d(v0.columns_as_1D(), v1.rows_as_1D());
    // TData denom = dot0d(v0, dot1d(M - M.T(), v1));
    if (std::abs(denom) < 1e-14) {
        auto sd = (std::stringstream() << denom).str();
        throw TriangleException(a, b, c, "barycentric coordinates encountered zero denominator: " + sd);
    }
    FixedArray<double, 2> pn = m.transform(p);
    FixedArray<double, 2> v2 = (pn - an);
    double d20 = dot0d(v2, v0);
    double d21 = dot0d(v2, v1);
    v = (d11 * d20 - d01 * d21) / denom;
    w = (d00 * d21 - d01 * d20) / denom;
    u = 1 - v - w;
}
