// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Permanent_Node_Contact.hpp"
#include <VehicleArena/Physics/Containers/Permanent_Contacts.hpp>
#include <VehicleArena/Scene_Graph/Elements/Scene_Node.hpp>

using namespace VA;

PermanentNodeContact::PermanentNodeContact(
    PermanentContacts& permanent_contacts,
    DanglingBaseClassRef<SceneNode> scene_node0,
    DanglingBaseClassRef<SceneNode> scene_node1,
    RigidBodyPulses& rbp0,
    RigidBodyPulses& rbp1)
    : rbp0_{ rbp0 }
    , rbp1_{ rbp1 }
    , permanent_contacts_{ permanent_contacts }
    , scene_node0_{ scene_node0 }
    , scene_node1_{ scene_node1 }
{
    scene_node0_->destruction_observers.add({ *this, CURRENT_SOURCE_LOCATION });
    scene_node1_->destruction_observers.add({ *this, CURRENT_SOURCE_LOCATION });
}

void PermanentNodeContact::notify_destroyed(SceneNode& destroyed_object) {
    if (&destroyed_object == &scene_node0_.get()) {
        scene_node1_->destruction_observers.remove({ *this, CURRENT_SOURCE_LOCATION });
    }
    if (&destroyed_object == &scene_node1_.get()) {
        scene_node0_->destruction_observers.remove({ *this, CURRENT_SOURCE_LOCATION });
    }
    permanent_contacts_.remove(*this);
}
