// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Destruction_Functions_Removeal_Tokens_Ref.hpp>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace VA {

class IPlayer;

class Drivers {
public:
    using PlayersMap = std::map<std::string, DestructionFunctionsTokensRef<IPlayer>>;

    Drivers();
    ~Drivers();
    void set_seats(std::vector<std::string> seats);
    bool seat_exists(const std::string& seat) const;
    bool seat_is_free(const std::string& seat);
    void add(std::string seat, DanglingBaseClassRef<IPlayer> player, SourceLocation loc);
    DanglingBaseClassPtr<IPlayer> try_get(const std::string& seat) const;
    void clear();
    const std::string* first_free_seat() const;
    const std::string* next_free_seat(const std::string& current_seat) const;
    const PlayersMap& players_map() const;
private:
    std::vector<std::string> seats_;
    std::set<std::string> seats_set_;
    PlayersMap players_;
};

}
