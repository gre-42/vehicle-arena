// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Default_Uninitialized.hpp>
#include <VehicleArena/Math/Fixed_Rodrigues.hpp>
#include <VehicleArena/Math/Transformation/Transformation_Matrix.hpp>

namespace VA {

template <class TRotation, class TPos, size_t tsize>
class OffsetAndTaitBryanAngles {
public:
    static OffsetAndTaitBryanAngles identity() {
        return OffsetAndTaitBryanAngles{
            fixed_zeros<TRotation>(),
            fixed_zeros<TPos>()};
    };
    OffsetAndTaitBryanAngles(Uninitialized)
        : rotation{ uninitialized }
        , position{ uninitialized }
    {}
    OffsetAndTaitBryanAngles(
        const FixedArray<TRotation, tsize>& rotation,
        const FixedArray<TPos, tsize>& position)
        : rotation{ rotation }
        , position{ position }
    {}
    OffsetAndTaitBryanAngles(
        const FixedArray<TRotation, tsize, tsize>& rotation,
        const FixedArray<TPos, tsize>& position)
        : rotation{ matrix_2_tait_bryan_angles(rotation) }
        , position{ position }
    {}

    TransformationMatrix<TRotation, TPos, tsize> to_matrix(
        const FixedArray<size_t, 3>& indices = {(size_t)0, (size_t)1, (size_t)2}) const
    {
        return TransformationMatrix<TRotation, TPos, tsize>{
            tait_bryan_angles_2_matrix(rotation, indices),
            position};
    }

    FixedArray<TRotation, tsize> rotation;
    FixedArray<TPos, tsize> position;

    template <class Archive>
    void serialize(Archive& archive) {
        archive(rotation);
        archive(position);
    }
};

template <class TRotation, class TPos, size_t tsize>
using UOffsetAndTaitBryanAngles = DefaultUnitialized<OffsetAndTaitBryanAngles<TRotation, TPos, tsize>>;

};
