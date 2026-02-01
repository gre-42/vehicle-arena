// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Asset_References.hpp"
#include <VehicleArena/Macro_Executor/Asset_Group_Replacement_Parameters.hpp>
#include <mutex>
#include <stdexcept>

using namespace VA;

AssetReferences::AssetReferences() = default;

AssetReferences::~AssetReferences() = default;

bool AssetReferences::contains(const std::string& group) const {
    std::shared_lock lock{mutex_};
    return replacement_parameters_.contains(group);
}

void AssetReferences::add(const std::string& group) {
    std::scoped_lock lock{mutex_};
    if (!replacement_parameters_.try_emplace(group).second) {
        throw std::runtime_error("Replacement parameter group \"" + group + "\" already exists");
    }
}

const AssetGroupReplacementParameters& AssetReferences::operator [] (const std::string& group) const
{
    std::shared_lock lock{mutex_};
    auto it = replacement_parameters_.find(group);
    if (it == replacement_parameters_.end()) {
        throw std::runtime_error("Could not find replacement parameter group \"" + group + '"');
    }
    return it->second;
}

AssetGroupReplacementParameters& AssetReferences::operator [] (const std::string& group)
{
    const AssetReferences& a = *this;
    return const_cast<AssetGroupReplacementParameters&>(a[group]);
}
