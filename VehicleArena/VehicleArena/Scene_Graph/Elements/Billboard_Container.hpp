// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Billboard_Id.hpp>
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Bvh.hpp>
#include <VehicleArena/Math/Fixed_Math.hpp>
#include <VehicleArena/Scene_Config/Scene_Precision.hpp>
#include <vector>

namespace VA {

template <class TPosition>
struct PositionAndYAngleAndBillboardId {
    FixedArray<TPosition, 3> position;
    BillboardId billboard_id;
    SceneDir yangle;
    inline const auto& primitive() const {
        return position;
    }
    inline PositionAndYAngleAndBillboardId& payload() {
        return *this;
    }
    inline const PositionAndYAngleAndBillboardId& payload() const {
        return *this;
    }
};
static_assert(sizeof(PositionAndYAngleAndBillboardId<HalfCompressedScenePos>) == 2 * 3 + 2 + 4);

inline PositionAndYAngleAndBillboardId<CompressedScenePos> operator + (
    const PositionAndYAngleAndBillboardId<HalfCompressedScenePos>& a,
    const FixedArray<CompressedScenePos, 3>& reference)
{
    return { a.position.casted<CompressedScenePos>() + reference, a.billboard_id, a.yangle };
}

inline PositionAndYAngleAndBillboardId<HalfCompressedScenePos> operator - (
    const PositionAndYAngleAndBillboardId<CompressedScenePos>& a,
    const FixedArray<CompressedScenePos, 3>& reference)
{
    auto p = a.position - reference;
    auto cp = p.casted<HalfCompressedScenePos>();
    if (any(cp.casted<CompressedScenePos>() != p)) {
        throw std::runtime_error("PositionAndYAngleAndBillboardId: Could not compress scene position");
    }
    return { cp, a.billboard_id, a.yangle };
}

template <class TPosition>
struct PositionAndBillboardId {
    FixedArray<TPosition, 3> position;
    BillboardId billboard_id;
    inline const auto& primitive() const
    {
        return position;
    }
    inline PositionAndBillboardId& payload() {
        return *this;
    }
    inline const PositionAndBillboardId& payload() const {
        return *this;
    }
};
static_assert(sizeof(PositionAndBillboardId<HalfCompressedScenePos>) == 8);

inline PositionAndBillboardId<CompressedScenePos> operator + (
    const PositionAndBillboardId<HalfCompressedScenePos>& a,
    const FixedArray<CompressedScenePos, 3>& reference)
{
    return { a.position.casted<CompressedScenePos>() + reference, a.billboard_id };
}

inline PositionAndBillboardId<HalfCompressedScenePos> operator - (
    const PositionAndBillboardId<CompressedScenePos>& a,
    const FixedArray<CompressedScenePos, 3>& reference)
{
    auto p = a.position - reference;
    auto cp = p.casted<HalfCompressedScenePos>();
    if (any(cp.casted<CompressedScenePos>() != p)) {
        throw std::runtime_error("PositionAndBillboardId: Could not compress scene position");
    }
    return { cp, a.billboard_id };
}

class BillboardContainer {
public:
    auto& add(const PositionAndYAngleAndBillboardId<CompressedScenePos>& pyb) {
        if (empty()) {
            reference_point_ = VA::center(pyb.primitive());
        }
        return pybs_.emplace_back(pyb - reference_point_);
    }
    auto& add(const PositionAndBillboardId<CompressedScenePos>& pb) {
        if (empty()) {
            reference_point_ = VA::center(pb.primitive());
        }
        return pbs_.emplace_back(pb - reference_point_);
    }
    void fill(auto& container) const {
        for (const auto& d : pybs_) {
            container.insert(d + reference_point_);
        }
        for (const auto& d : pbs_) {
            container.insert(d + reference_point_);
        }
    }
    bool visit(const auto& aabb, const auto& pyb_visitor, const auto& pb_visitor) const {
        for (const auto& d : pybs_) {
            auto ud = d + reference_point_;
            if (intersects(aabb, ud.primitive())) {
                if (!pyb_visitor(ud.payload())) {
                    return false;
                }
            }
        }
        for (const auto& d : pbs_) {
            auto ud = d + reference_point_;
            if (intersects(aabb, ud.primitive())) {
                if (!pb_visitor(ud.payload())) {
                    return false;
                }
            }
        }
        return true;
    }
    bool visit_all(const auto& pyb_visitor, const auto& pb_visitor) const {
        for (const auto& d : pybs_) {
            if (!pyb_visitor(d + reference_point_)) {
                return false;
            }
        }
        for (const auto& d : pbs_) {
            if (!pb_visitor(d + reference_point_)) {
                return false;
            }
        }
        return true;
    }
    bool visit_pairs(const auto& aabb, const auto& pyb_visitor, const auto& pb_visitor) const {
        for (const auto& d : pybs_) {
            auto ud = d + reference_point_;
            if (intersects(aabb, ud.primitive())) {
                if (!pyb_visitor(ud)) {
                    return false;
                }
            }
        }
        for (const auto& d : pbs_) {
            auto ud = d + reference_point_;
            if (intersects(aabb, ud.primitive())) {
                if (!pb_visitor(ud)) {
                    return false;
                }
            }
        }
        return true;
    }
    void print(std::ostream& ostr, size_t rec = 0) const {
        for (const auto& d : pybs_) {
            VA::print((d + reference_point_).primitive(), ostr, rec + 1);
        }
        for (const auto& d : pbs_) {
            VA::print((d + reference_point_).primitive(), ostr, rec + 1);
        }
    }
    bool empty() const {
        return pybs_.empty() && pbs_.empty();
    }
    size_t size() const {
        return pybs_.size() + pbs_.size();
    }
    void clear() {
        pybs_.clear();
        pbs_.clear();
    }
private:
    FixedArray<CompressedScenePos, 3> reference_point_ = uninitialized;
    std::vector<PositionAndYAngleAndBillboardId<HalfCompressedScenePos>> pybs_;
    std::vector<PositionAndBillboardId<HalfCompressedScenePos>> pbs_;
};

}
