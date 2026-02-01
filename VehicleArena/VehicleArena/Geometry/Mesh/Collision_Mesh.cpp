// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Collision_Mesh.hpp"
#include <VehicleArena/Geometry/Colored_Vertex.hpp>
#include <VehicleArena/Geometry/Interfaces/IIntersectable.hpp>
#include <VehicleArena/Geometry/Mesh/Colored_Vertex_Array.hpp>
#include <VehicleArena/Geometry/Mesh/Typed_Mesh.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

using namespace VA;

template <class TData>
CollisionMesh::CollisionMesh(const ColoredVertexArray<TData>& mesh)
    : name{ mesh.name.full_name() }
{
    quads.reserve(mesh.quads.size());
    mesh.quads_sphere(quads);

    triangles.reserve(mesh.triangles.size());
    mesh.triangles_sphere(triangles);

    lines = mesh.lines_sphere();
}

CollisionMesh::CollisionMesh(
    std::string name,
    TypedMesh<std::shared_ptr<IIntersectable>> intersectable)
    : name{ std::move(name) }
    , intersectable{ std::move(intersectable) }
{}

CollisionMesh::~CollisionMesh() = default;

namespace VA {

template CollisionMesh::CollisionMesh(const ColoredVertexArray<float>&);
template CollisionMesh::CollisionMesh(const ColoredVertexArray<CompressedScenePos>&);

}
