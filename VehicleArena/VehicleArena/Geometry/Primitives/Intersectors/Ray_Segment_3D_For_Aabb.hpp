// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Ray_Segment_3D.hpp>

namespace VA {

template <class TDir, class TPos>
class RaySegment3DForAabb: public RaySegment3D<TDir, TPos> {
public:
    using RaySegment3D<TDir, TPos>::start;
    using RaySegment3D<TDir, TPos>::direction;
    using RaySegment3D<TDir, TPos>::length;
    using RaySegment3D<TDir, TPos>::stop;
    using RaySegment3D<TDir, TPos>::intersects;
    explicit RaySegment3DForAabb(const RaySegment3D<TDir, TPos>& rs3)
        : RaySegment3D<TDir, TPos>{ rs3 }
        , inv_direction{ TDir(1) / rs3.direction }
    {}
    bool intersects(const AxisAlignedBoundingBox<TPos, 3>& aabb) const {
        if (aabb.contains(start) || aabb.contains(stop())) {
            return true;
        }
        auto contains1d = [&](const TPos& t, size_t i) {
            auto x = start(i) + direction(i) * t;
            return (x >= aabb.min(i)) && (x <= aabb.max(i));
        };
        auto contains2d = [&](const TPos& t, size_t i1, size_t i2) {
            if ((t < 0) || (t > length)) {
                return false;
            }
            return contains1d(t, i1) && contains1d(t, i2);
        };
        auto intersects0 = [&](size_t i0, size_t i1, size_t i2) {
            auto inv_c = inv_direction(i0);
            if (std::abs(inv_c) > 1e12) {
                return false;
            }
            return
                contains2d((aabb.min(i0) - start(i0)) * inv_c, i1, i2) ||
                contains2d((aabb.max(i0) - start(i0)) * inv_c, i1, i2);
        };
        return intersects0(0, 1, 2) || intersects0(1, 2, 0) || intersects0(2, 0, 1);
    }
private:
    FixedArray<TDir, 3> inv_direction;
};

template <class TDir, class TPos>
inline bool intersects(
    const RaySegment3DForAabb<TDir, TPos>& a,
    const AxisAlignedBoundingBox<TPos, 3>& b)
{
    return a.intersects(b);
}

inline bool intersects(
    const RaySegment3DForAabb<ScenePos, ScenePos>& a,
    const AxisAlignedBoundingBox<CompressedScenePos, 3>& b)
{
    return a.intersects(b.casted<ScenePos>());
}

}
