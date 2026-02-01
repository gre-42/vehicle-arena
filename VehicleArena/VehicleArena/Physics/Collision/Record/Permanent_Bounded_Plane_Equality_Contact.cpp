// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Permanent_Bounded_Plane_Equality_Contact.hpp"
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Physics/Collision/Resolve/Constraints.hpp>
#include <VehicleArena/Physics/Containers/Permanent_Contacts.hpp>
#include <VehicleArena/Physics/Rigid_Body/Rigid_Body_Pulses.hpp>
#include <VehicleArena/Scene_Config/Physics_Engine_Config.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

PermanentBoundedPlaneEqualityContact::PermanentBoundedPlaneEqualityContact(
    PermanentContacts& permanent_contacts,
    DanglingBaseClassRef<SceneNode> scene_node0,
    DanglingBaseClassRef<SceneNode> scene_node1,
    RigidBodyPulses& rbp0,
    RigidBodyPulses& rbp1,
    const FixedArray<ScenePos, 3>& p0,
    const FixedArray<ScenePos, 3>& p1,
    const FixedArray<float, 3>& normal0)
    : PermanentNodeContact{
        permanent_contacts,
        scene_node0,
        scene_node1,
        rbp0,
        rbp1
    }
    , p0_{ p0 }
    , p1_{ p1 }
    , normal0_{ normal0 }
{}

void PermanentBoundedPlaneEqualityContact::extend_contact_infos(
    const PhysicsEngineConfig& cfg,
    std::list<std::unique_ptr<IContactInfo>>& contact_infos)
{
    auto T0 = rbp0_.abs_transformation();
    auto T1 = rbp1_.abs_transformation();
    contact_infos.push_back(std::make_unique<PlaneContactInfo2>(
        rbp0_,
        rbp1_,
        BoundedPlaneEqualityConstraint{
            PlaneEqualityConstraint{
                .pec = PointEqualityConstraint{
                    .p0 = T0.transform(p0_),
                    .p1 = T1.transform(p1_),
                    .beta = cfg.point_equality_beta
                },
                .plane_normal = T0.rotate(normal0_)
            } }));
}
