// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Array/Fixed_Array.hpp>
#include <VehicleArena/Physics/Collision/Record/Permanent_Node_Contact.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

namespace VA {

class RigidBodyPulses;
class PermanentContacts;
class SceneNode;

class PermanentLineContact: public PermanentNodeContact {
public:
    PermanentLineContact(
        PermanentContacts& permanent_contacts,
        DanglingBaseClassRef<SceneNode> scene_node0,
        DanglingBaseClassRef<SceneNode> scene_node1,
        RigidBodyPulses& rbp0,
        RigidBodyPulses& rbp1,
        const FixedArray<ScenePos, 3>& p0,
        const FixedArray<ScenePos, 3>& p1,
        const FixedArray<float, 3>& line0);
    
    // IPermanentContact
    virtual void extend_contact_infos(
        const PhysicsEngineConfig& cfg,
        std::list<std::unique_ptr<IContactInfo>>& contact_infos) override;
private:
    FixedArray<ScenePos, 3> p0_;
    FixedArray<ScenePos, 3> p1_;
    FixedArray<float, 3> line0_;
};

}
