// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Permanent_Contacts.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Physics/Collision/Record/IPermanent_Contact.hpp>
#include <stdexcept>

using namespace VA;

PermanentContacts::PermanentContacts() = default;

PermanentContacts::~PermanentContacts() = default;

void PermanentContacts::insert(std::unique_ptr<IPermanentContact>&& permanent_contact) {
    if (!permanent_contacts_.insert(std::move(permanent_contact)).second) {
        throw std::runtime_error("Could not insert permanent contact");
    }
}

void PermanentContacts::remove(const IPermanentContact& permanent_contact) {
    auto it = permanent_contacts_.find(&permanent_contact);
    if (it == permanent_contacts_.end()) {
        verbose_abort("Cannot find permanent contact to be deleted");
    }
    permanent_contacts_.erase(it);
}

void PermanentContacts::extend_contact_infos(
    const PhysicsEngineConfig& cfg,
    std::list<std::unique_ptr<IContactInfo>>& contact_infos) const
{
    for (const auto& pc : permanent_contacts_) {
        pc->extend_contact_infos(cfg, contact_infos);
    }
}
