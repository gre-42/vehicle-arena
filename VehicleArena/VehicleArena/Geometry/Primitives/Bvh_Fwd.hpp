// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <cstdint>
#include <list>
#include <vector>

namespace VA {

enum class BvhThreadSafety {
    NOT_THREAD_SAFE,
    THREAD_SAFE
};

template <class TPosition, size_t tndim, class TData, BvhThreadSafety thread_safety = BvhThreadSafety::NOT_THREAD_SAFE>
class GenericBvh;

template <class TPosition, size_t tndim, class TPayload>
class AabbAndPayload;

template <class TPosition, size_t tndim, class TPayload>
class PointAndPayload;

template <class TPosition, size_t tndim>
class PointWithoutPayload;

template <class TContainer>
class PayloadContainer;

template <class TPosition, size_t tndim, class TSmallContainer, class TLargeContainer>
class CompressedPayloadContainer;

template <class TPosition, size_t tndim, class TPayload, BvhThreadSafety thread_safety = BvhThreadSafety::NOT_THREAD_SAFE>
using Bvh = GenericBvh<
    TPosition,
    tndim,
    PayloadContainer<std::list<AabbAndPayload<TPosition, tndim, TPayload>>>,
    thread_safety>;

template <class TPosition, size_t tndim, class TPayload>
using PointAndPayloadVectorBvh = GenericBvh<
    TPosition,
    tndim,
    PayloadContainer<std::vector<PointAndPayload<TPosition, tndim, TPayload>>>>;

template <class TPosition, size_t tndim>
using PointWithoutPayloadVectorBvh = GenericBvh<
    TPosition,
    tndim,
    PayloadContainer<std::vector<PointWithoutPayload<TPosition, tndim>>>>;

template <class TPosition, class TCompressedPosition, class TPayload, class TCompressedPayload, size_t tndim>
using CompressedBvh = GenericBvh<
    TPosition,
    tndim,
    CompressedPayloadContainer<
        TPosition,
        tndim,
        std::list<AabbAndPayload<TCompressedPosition, tndim, TCompressedPayload>>,
        std::list<AabbAndPayload<TPosition, tndim, TPayload>>>>;

template <class TPosition, class TCompressedPosition, size_t tndim>
using CompressedPointVectorBvh = GenericBvh<
    TPosition,
    tndim,
    CompressedPayloadContainer<
        TPosition,
        tndim,
        std::vector<PointWithoutPayload<TCompressedPosition, tndim>>,
        std::vector<PointWithoutPayload<TPosition, tndim>>>>;

}
