// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Shading.hpp"
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>

using namespace VA;

namespace KnownArgs {
BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(emissive);
DECLARE_ARGUMENT(ambient);
DECLARE_ARGUMENT(diffuse);
DECLARE_ARGUMENT(specular);
DECLARE_ARGUMENT(specular_exponent);
DECLARE_ARGUMENT(reflectance);
DECLARE_ARGUMENT(fresnel);
DECLARE_ARGUMENT(fog_distances);
DECLARE_ARGUMENT(fog_ambient);
}

void VA::from_json(const nlohmann::json& j, Shading& shading) {
    JsonView jv{ j };
    jv.validate(KnownArgs::options);
    shading.emissive = jv.at<EFixedArray<float, 3>>(KnownArgs::emissive, { 0.f, 0.f, 0.f });
    shading.ambient = jv.at<EFixedArray<float, 3>>(KnownArgs::ambient, { 1.f, 1.f, 1.f });
    shading.diffuse = jv.at<EFixedArray<float, 3>>(KnownArgs::diffuse, { 0.8f, 0.8f, 0.8f });
    shading.specular = jv.at<EFixedArray<float, 3>>(KnownArgs::specular, { 0.5f, 0.5f, 0.5f });
    shading.specular_exponent = jv.at<float>(KnownArgs::specular_exponent, 4.f);
    shading.reflectance = jv.at<EFixedArray<float, 3>>(KnownArgs::reflectance, { 0.f, 0.f, 0.f });
    shading.fresnel = jv.at<FresnelAndAmbient>(KnownArgs::fresnel, FresnelAndAmbient{});
    shading.fog_distances = jv.at<EFixedArray<float, 2>>(KnownArgs::fog_distances, default_step_distances);
    shading.fog_ambient = jv.at<EFixedArray<float, 3>>(KnownArgs::fog_ambient, { -1.f, -1.f, -1.f });
}
