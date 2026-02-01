// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Ridge_Intersection_Points_Bvh.hpp"
#include <VehicleArena/Scene_Config/Physics_Engine_Config.hpp>

using namespace VA;

RidgeIntersectionPointsBvh::RidgeIntersectionPointsBvh(const PhysicsEngineConfig& cfg)
    : bvh_{{cfg.bvh_max_size, cfg.bvh_max_size, cfg.bvh_max_size}, cfg.bvh_levels}
    , radius_{cfg.intersection_point_radius}
{}

void RidgeIntersectionPointsBvh::insert(const FixedArray<ScenePos, 3>& intersection_point) {
    auto ip = intersection_point.casted<CompressedScenePos>();
    bvh_.insert(AxisAlignedBoundingBox<CompressedScenePos, 3>::from_point(ip), ip);
}

bool RidgeIntersectionPointsBvh::has_neighbor(const FixedArray<ScenePos, 3>& intersection_point)
{
    auto ip = intersection_point.casted<CompressedScenePos>();
    return bvh_.has_neighbor2(
        ip,
        (CompressedScenePos)radius_,
        [&ip](const FixedArray<CompressedScenePos, 3>& candidate){
            return sum(squared(ip - candidate));
        });
}
