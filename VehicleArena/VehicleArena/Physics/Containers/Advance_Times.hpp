// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Misc/Source_Location.hpp>
#include <list>
#include <map>
#include <memory>
#include <mutex>

namespace VA {

class IAdvanceTime;
template <class T>
class DestructionFunctionsTokensRef;
template <class T>
class DanglingBaseClassRef;
struct StaticWorld;

class AdvanceTimes {
public:
    AdvanceTimes();
    ~AdvanceTimes();
    void add_advance_time(const DanglingBaseClassRef<IAdvanceTime>& advance_time, SourceLocation loc);
    void delete_advance_time(const IAdvanceTime& advance_time, SourceLocation loc);
    void advance_time(float dt, const StaticWorld& world);
    bool empty() const;
private:
    bool advancing_time_;
    std::list<std::pair<std::unique_ptr<DestructionFunctionsTokensRef<IAdvanceTime>>, SourceLocation>> advance_times_;
};

}
