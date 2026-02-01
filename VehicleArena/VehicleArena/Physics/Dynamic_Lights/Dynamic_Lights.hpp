// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Graph/Interfaces/IDynamic_Lights.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <chrono>
#include <cstddef>
#include <functional>
#include <set>

namespace VA {

template <typename TData, size_t... tshape>
class FixedArray;
class DynamicLightDb;
class IDynamicLight;

class DynamicLights: public IDynamicLights {
public:
    explicit DynamicLights(const DynamicLightDb& db);
    virtual ~DynamicLights() override;
    std::unique_ptr<IDynamicLight> instantiate(
        const std::string& name,
        const std::function<FixedArray<ScenePos, 3>()>& get_position,
        std::chrono::steady_clock::time_point time);
    void erase(IDynamicLight& light);

    // IDynamicLights
    virtual void append_time(std::chrono::steady_clock::time_point time) override;
    virtual void set_time(std::chrono::steady_clock::time_point time) override;
    virtual FixedArray<float, 3> get_color(const FixedArray<ScenePos, 3>& target_position) const override;

private:
    const DynamicLightDb& db_;
    std::set<IDynamicLight*> instances_;
    mutable FastMutex mutex_;
};

}
