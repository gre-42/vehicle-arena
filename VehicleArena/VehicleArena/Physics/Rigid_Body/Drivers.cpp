// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Drivers.hpp"
#include <VehicleArena/Memory/Recursive_Deletion.hpp>
#include <VehicleArena/Physics/Interfaces/IPlayer.hpp>
#include <stdexcept>

using namespace VA;

Drivers::Drivers() = default;

Drivers::~Drivers() = default;

void Drivers::set_seats(std::vector<std::string> seats) {
    seats_ = std::move(seats);
    seats_set_ = std::set(seats_.begin(), seats_.end());
}

bool Drivers::seat_exists(const std::string& seat) const {
    return seats_set_.contains(seat);
}

bool Drivers::seat_is_free(const std::string& seat) {
    if (!seats_set_.contains(seat)) {
        throw std::runtime_error("Unknown seat: \"" + seat + '"');
    }
    return !players_.contains(seat);
}

void Drivers::add(
    std::string seat,
    DanglingBaseClassRef<IPlayer> player,
    SourceLocation loc)
{
    if (!seats_set_.contains(seat)) {
        throw std::runtime_error("Unknown seat: \"" + seat + '"');
    }
    auto it = players_.try_emplace(std::move(seat), player, player->on_clear_vehicle(), loc);
    if (!it.second) {
        throw std::runtime_error("Player with seat \"" + seat + "\" already exists");
    }
    it.first->second.on_destroy(
        [this, it](){ players_.erase(it.first); },
        CURRENT_SOURCE_LOCATION);
}

DanglingBaseClassPtr<IPlayer> Drivers::try_get(const std::string& seat) const {
    if (!seats_set_.contains(seat)) {
        throw std::runtime_error("Unknown seat: \"" + seat + '"');
    }
    auto it = players_.find(seat);
    if (it == players_.end()) {
        return nullptr;
    }
    return it->second.object().ptr();
}

void Drivers::clear() {
    clear_container_recursively(players_);
}

const std::string* Drivers::first_free_seat() const {
    for (const auto& seat : seats_) {
        if (!players_.contains(seat)) {
            return &seat;
        }
    }
    return nullptr;
}

const std::string* Drivers::next_free_seat(const std::string& current_seat) const {
    bool found = false;
    for (const auto& seat : seats_) {
        if (found) {
            if (!players_.contains(seat)) {
                return &seat;
            }
        } else {
            found |= (seat == current_seat);
        }
    }
    return nullptr;
}

const Drivers::PlayersMap& Drivers::players_map() const {
    return players_;
}

