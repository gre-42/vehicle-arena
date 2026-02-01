// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collide_Concave_Triangles.hpp"
#include <VehicleArena/Physics/Collision/Record/Handle_Line_Triangle_Intersection.hpp>
#include <VehicleArena/Physics/Collision/Record/Intersection_Scene.hpp>
#include <VehicleArena/Physics/Collision/Record/Ridge_Intersection_Points_Bvh.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Vehicle.hpp>

using namespace VA;

void VA::collide_concave_triangles(
    const PhysicsEngineConfig& cfg,
    std::unordered_map<RigidBodyVehicle*, std::list<IntersectionSceneAndContact>>& concave_t0_intersections,
    std::unordered_map<RigidBodyVehicle*, std::list<FixedArray<ScenePos, 3>>>& ridge_intersection_points)
{
    for (auto& [rb1, cs] : concave_t0_intersections) {
        auto it = ridge_intersection_points.find(rb1);
        if (it != ridge_intersection_points.end()) {
            RidgeIntersectionPointsBvh bvh{ cfg };
            for (const auto& p : it->second) {
                bvh.insert(p);
            }
            for (const auto& c : cs) {
                if (bvh.has_neighbor(c.iinfo.intersection_point)) {
                    continue;
                }
                handle_line_triangle_intersection(c.scene, c.iinfo);
            }
        } else {
            for (const auto& c : cs) {
                handle_line_triangle_intersection(c.scene, c.iinfo);
            }
        }
    }
}
