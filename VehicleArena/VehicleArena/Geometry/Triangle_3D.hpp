// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <Mlib/Array/Fixed_Array.hpp>
#include <random>

namespace Mlib {

template <class TDir, class TPos, size_t tnvertices>
class ConvexPolygon3D;
template <class TPos, size_t tndim>
class BoundingSphere;
template <class TData, size_t tndim>
class AxisAlignedBoundingBox;
template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TPos>
struct ColoredVertex;

template <class TPos>
class Triangle3D {
public:
    using I = funpack_t<TPos>;
    explicit Triangle3D(const FixedArray<TPos, 3, 3>& vertices);
    template <class TPos2>
    explicit Triangle3D(const FixedArray<ColoredVertex<TPos2>, 3>& vertices);
    template <class TPos2, class TPosTransform>
    Triangle3D(
        const FixedArray<ColoredVertex<TPos2>, 3>& vertices,
        const TransformationMatrix<float, TPosTransform, 3>& transformation);
    const FixedArray<TPos, 3, 3>& vertices() const;
    ConvexPolygon3D<I, TPos, 3> polygon() const;
    BoundingSphere<TPos, 3> bounding_sphere(std::minstd_rand& rng) const;
    AxisAlignedBoundingBox<TPos, 3> aabb() const;
private:
    const FixedArray<TPos, 3, 3> vertices_;
};

}
