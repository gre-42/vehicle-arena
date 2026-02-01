// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <chrono>
#include <cstddef>
#include <string>

namespace VA {

template <class TDir, class TPos, size_t n>
class TransformationMatrix;
template <class TData, size_t n>
struct FixedScaledUnitVector;

class SceneNodeResources;

class DynamicWorld {
    DynamicWorld(const DynamicWorld&) = delete;
    DynamicWorld& operator = (const DynamicWorld&) = delete;
public:
    DynamicWorld(const SceneNodeResources& scene_node_resources, VariableAndHash<std::string> name);

    const TransformationMatrix<double, double, 3>* get_geographic_mapping() const;
    const TransformationMatrix<double, double, 3>* get_inverse_geographic_mapping() const;
    const FixedScaledUnitVector<float, 3>* get_gravity() const;
    const FixedScaledUnitVector<float, 3>* get_wind() const;
private:
    const SceneNodeResources& scene_node_resources_;
    VariableAndHash<std::string> name_;
    VariableAndHash<std::string> inverse_name_;
};

}
