// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Geometry/Primitives/Axis_Aligned_Bounding_Box.hpp>
#include <VehicleArena/Geometry/Primitives/Extremal_Bounding_Volume.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <variant>

namespace VA {

template <class TData, size_t tndim>
class ExtremalAxisAlignedBoundingBox {
public:
    ExtremalAxisAlignedBoundingBox(ExtremalBoundingVolume other)
        : data_{ other }
    {}
    ExtremalAxisAlignedBoundingBox(const AxisAlignedBoundingBox<TData, tndim>& other)
        : data_{ other }
    {}
    ExtremalAxisAlignedBoundingBox& operator = (ExtremalBoundingVolume other) {
        data_ = other;
        return *this;
    }
    ExtremalAxisAlignedBoundingBox& operator = (const AxisAlignedBoundingBox<TData, tndim>& other) {
        data_ = other;
        return *this;
    }
    void extend(const ExtremalAxisAlignedBoundingBox<TData, tndim>& other) {
        if (auto* d = std::get_if<ExtremalBoundingVolume>(&data_)) {
            switch (*d) {
                case ExtremalBoundingVolume::EMPTY:
                    data_ = other.data_;
                    return;
                case ExtremalBoundingVolume::FULL:
                    return;
            }
            verbose_abort("Unknown bounding volume: " + std::to_string((int)(*d)));
        }
        if (const auto* d = std::get_if<ExtremalBoundingVolume>(&other.data_)) {
            switch (*d) {
                case ExtremalBoundingVolume::EMPTY:
                    return;
                case ExtremalBoundingVolume::FULL:
                    data_ = ExtremalBoundingVolume::FULL;
                    return;
            }
            verbose_abort("Unknown bounding volume: " + std::to_string((int)(*d)));
        }
        if (auto* lhs = std::get_if<AxisAlignedBoundingBox<TData, tndim>>(&data_)) {
            if (auto* rhs = std::get_if<AxisAlignedBoundingBox<TData, tndim>>(&other.data_)) {
                lhs->extend(*rhs);
                return;
            }
        }
        verbose_abort("Unknown bounding volume");
    }
    template <class TResult>
    ExtremalAxisAlignedBoundingBox<TResult, tndim> casted() const {
        if (const auto* d = std::get_if<ExtremalBoundingVolume>(&data_)) {
            return *d;
        }
        if (const auto* d = std::get_if<AxisAlignedBoundingBox<TData, tndim>>(&data_)) {
            return d->template casted<TResult>();
        }
        verbose_abort("Unknown bounding volume");
    }
    bool empty() const {
        if (const auto* d = std::get_if<ExtremalBoundingVolume>(&data_)) {
            return *d == ExtremalBoundingVolume::EMPTY;
        }
        return false;
    }
    bool full() const {
        if (const auto* d = std::get_if<ExtremalBoundingVolume>(&data_)) {
            return *d == ExtremalBoundingVolume::FULL;
        }
        return false;
    }
    const AxisAlignedBoundingBox<TData, tndim>& data() const {
        if (const auto* d = std::get_if<AxisAlignedBoundingBox<TData, tndim>>(&data_)) {
            return *d;
        }
        verbose_abort("ExtremalAxisAlignedBoundingBox is empty or full");
    }
private:
    std::variant<ExtremalBoundingVolume, AxisAlignedBoundingBox<TData, tndim>> data_;
};

}
