// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Shade_Auto.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Vertex_Normals_With_Seams.hpp>
#include <VehicleArena/Geometry/Triangle_Normal.hpp>

using namespace VA;

template <class TPos>
void VA::shade_auto(ColoredVertexArray<TPos>& cva, float seam_angle)
{
    if (seam_angle == 0.f) {
        for (auto& t : cva.triangles) {
            using Triangle = FixedArray<TPos, 3, 3>;
            auto normal = triangle_normal(funpack(Triangle{
                t(0).position,
                t(1).position,
                t(2).position})).template casted<float>();
            t(0).normal = normal;
            t(1).normal = normal;
            t(2).normal = normal;
        }
    } else {
        float seam_threshold = std::cos(seam_angle);
        VertexNormalsWithSeams<TPos, float> vns;
        vns.add_triangles(cva.triangles.begin(), cva.triangles.end());
        for (auto& t : cva.triangles) {
            t = vns.triangle(t, seam_threshold);
        }
    }
}

template void VA::shade_auto<float>(
    ColoredVertexArray<float>& cvas,
    float seam_angle);
template void VA::shade_auto<CompressedScenePos>(
    ColoredVertexArray<CompressedScenePos>& cvas,
    float seam_angle);
