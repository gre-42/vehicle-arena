// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Weapon_Cycle.hpp"
#include <VehicleArena/Physics/Bullets/Bullet_Properties.hpp>
#include <VehicleArena/Physics/Misc/Inventory.hpp>
#include <VehicleArena/Physics/Misc/When_To_Equip.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <stdexcept>

using namespace VA;

float WeaponInfo::score(double distance_to_target) const {
    if (cool_down == 0) {
        throw std::runtime_error("Weapon has cooldown 0");
    }
    if ((distance_to_target < range_min) ||
        (distance_to_target > range_max))
    {
        return -INFINITY;
    }
    return bullet_properties.damage / (cool_down / seconds);
}

WeaponCycle::WeaponCycle() = default;

WeaponCycle::~WeaponCycle() = default;

void WeaponCycle::modify_node() {
    if (equipped_ != desired_) {
        auto it = weapon_infos_.find(desired_.weapon_name);
        if (it == weapon_infos_.end()) {
            throw std::runtime_error("Inventory does not have information about a weapon with name \"" + desired_.weapon_name + '"');
        }
        if (it->second.create_weapon) {
            it->second.create_weapon();
        }
        if (it->second.create_externals && desired_.player_name.has_value()) {
            it->second.create_externals(*desired_.player_name);
        }
        equipped_ = desired_;
    }
}

void WeaponCycle::create_externals(const VariableAndHash<std::string>& player_name) {
    auto it = weapon_infos_.find(equipped_.weapon_name);
    if (it == weapon_infos_.end()) {
        throw std::runtime_error("Inventory does not have information about a weapon with name \"" + equipped_.weapon_name + '"');
    }
    if (it->second.create_externals) {
        it->second.create_externals(player_name);
    }
}

void WeaponCycle::add_weapon(std::string weapon_name, const WeaponInfo& weapon_info)
{
    if (!weapon_infos_.try_emplace(std::move(weapon_name), weapon_info).second)  {
        throw std::runtime_error("Inventory already has information about a weapon with name \"" + weapon_name + '"');
    }
}

void WeaponCycle::set_desired_weapon(
    std::optional<VariableAndHash<std::string>> player_name,
    std::string weapon_name,
    WhenToEquip when_to_equip)
{
    desired_ = { std::move(player_name), std::move(weapon_name) };
    if (when_to_equip == WhenToEquip::EQUIP_INSTANTLY) {
        modify_node();
    }
}

void WeaponCycle::equip_next_weapon(std::optional<VariableAndHash<std::string>> player_name) {
    if (weapon_infos_.empty()) {
        return;
    }
    auto it = weapon_infos_.find(desired_.weapon_name);
    if (it == weapon_infos_.end()) {
        desired_.weapon_name = weapon_infos_.begin()->first;
    } else {
        ++it;
        if (it == weapon_infos_.end()) {
            return;
        }
        desired_.weapon_name = it->first;
    }
    desired_.player_name = std::move(player_name);
}

void WeaponCycle::equip_previous_weapon(std::optional<VariableAndHash<std::string>> player_name) {
    if (weapon_infos_.empty()) {
        return;
    }
    auto it = weapon_infos_.find(desired_.weapon_name);
    if (it == weapon_infos_.end()) {
        desired_.weapon_name = weapon_infos_.rbegin()->first;
    } else if (it != weapon_infos_.begin()) {
        --it;
        desired_.weapon_name = it->first;
    }
    desired_.player_name = std::move(player_name);
}

InventoryItem WeaponCycle::ammo_type() const {
    auto it = weapon_infos_.find(equipped_.weapon_name);
    if (it == weapon_infos_.end()) {
        throw std::runtime_error("Inventory does not have information about a weapon with name \"" + equipped_.weapon_name + '"');
    }
    return it->second.ammo_type;
}

const std::string& WeaponCycle::weapon_name() const {
    if (equipped_.weapon_name.empty()) {
        throw std::runtime_error("No weapon is equpped");
    }
    return equipped_.weapon_name;
}

const std::map<std::string, WeaponInfo>& WeaponCycle::weapon_infos() const {
    return weapon_infos_;
}

std::ostream& VA::operator << (std::ostream& ostr, const WeaponCycle& wc) {
    for (const auto& [name, _] : wc.weapon_infos_) {
        ostr << name << '\n';
    }
    return ostr;
}
