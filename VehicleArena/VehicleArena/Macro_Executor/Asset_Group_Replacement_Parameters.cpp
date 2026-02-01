// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Asset_Group_Replacement_Parameters.hpp"
#include <VehicleArena/Geometry/Interfaces/IAsset_Loader.hpp>
#include <VehicleArena/Macro_Executor/Macro_Line_Executor.hpp>
#include <VehicleArena/Macro_Executor/Replacement_Parameter.hpp>
#include <mutex>
#include <stdexcept>

using namespace VA;

AssetGroupReplacementParameters::AssetGroupReplacementParameters() = default;

AssetGroupReplacementParameters::~AssetGroupReplacementParameters() = default;

void AssetGroupReplacementParameters::insert_if_active(
    const std::string& filename,
    const MacroLineExecutor& mle)
{
    auto rp = ReplacementParameterAndFilename::from_json(filename);
    if (!mle.eval(rp.rp.required.fixed, rp.rp.database)) {
        return;
    }
    auto mlecd = mle.changed_script_filename(filename);
    if (rp.rp.on_init != nlohmann::detail::value_t::null) {
        mlecd(rp.rp.on_init, nullptr);
    }
    insert(std::move(rp));
}

void AssetGroupReplacementParameters::insert(ReplacementParameterAndFilename&& rp) {
    std::unique_lock lock{mutex_};
    if (!replacement_parameters_.try_emplace(rp.rp.id, std::move(rp)).second) {
        throw std::runtime_error("Asset with id \"" + rp.rp.id + "\" already exists");
    }
}

void AssetGroupReplacementParameters::merge_into_database(const std::string& id, const JsonMacroArguments& params) {
    std::unique_lock lock{mutex_};
    auto it = replacement_parameters_.find(id);
    if (it == replacement_parameters_.end()) {
        throw std::runtime_error("Asset with id \"" + id + "\" does not exist");
    }
    it->second.rp.database.merge(params);
}

const ReplacementParameterAndFilename& AssetGroupReplacementParameters::at(const std::string& id) const {
    std::shared_lock lock{mutex_};
    auto it = replacement_parameters_.find(id);
    if (it == replacement_parameters_.end()) {
        throw std::runtime_error("Could not find asset with id \"" + id + "\" in the asset group");
    }
    return it->second;
}

std::map<std::string, ReplacementParameterAndFilename>::const_iterator AssetGroupReplacementParameters::begin() const {
    return replacement_parameters_.begin();
}

std::map<std::string, ReplacementParameterAndFilename>::const_iterator AssetGroupReplacementParameters::end() const {
    return replacement_parameters_.end();
}

void AssetGroupReplacementParameters::add_asset_loader(std::unique_ptr<IAssetLoader>&& loader) {
    std::shared_lock lock{mutex_};
    asset_loaders_.emplace_back(std::move(loader));
}

const std::list<std::unique_ptr<IAssetLoader>>& AssetGroupReplacementParameters::loaders() const {
    std::shared_lock lock{mutex_};
    return asset_loaders_;
}
