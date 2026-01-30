// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Transformation_Matrix_Json.hpp"
#include <VehicleArena/Json/Json_View.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>
#include <VehicleArena/Misc/Argument_List.hpp>
#include <VehicleArena/Physics/Units.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>

using namespace VA;

namespace KnownArgs {

BEGIN_ARGUMENT_LIST;
DECLARE_ARGUMENT(position);
DECLARE_ARGUMENT(rotation);
DECLARE_ARGUMENT(scale);

}

template <class TDir, class TPos, size_t n>
TransformationMatrix<TDir, TPos, n> VA::transformation_matrix_from_json(const nlohmann::json& j) {
    JsonView jv{ j };
    jv.validate(KnownArgs::options);
    if constexpr (n == 3) {
        auto R = tait_bryan_angles_2_matrix(jv.at<EFixedArray<TDir, 3>>(KnownArgs::rotation) * (TDir)degrees);
        auto scale = jv.at<TDir>(KnownArgs::scale);
        using I = funpack_t<TPos>;
        auto position = (jv.at<EFixedArray<I, 3>>(KnownArgs::position) * (I)meters).template casted<TPos>();
        return { R * scale, position };
    } else {
        static_assert(n == 3, "Unsupported matrix dimension");
    }
}

namespace VA {

template TransformationMatrix<float, double, 3> VA::transformation_matrix_from_json(const nlohmann::json& j);
template TransformationMatrix<float, CompressedScenePos, 3> VA::transformation_matrix_from_json(const nlohmann::json& j);

}
