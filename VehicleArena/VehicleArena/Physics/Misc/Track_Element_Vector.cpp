// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Track_Element_Vector.hpp"
#include <VehicleArena/Physics/Misc/Track_Element_Extended.hpp>

using namespace VA;

TrackElementVector::TrackElementVector(std::vector<std::vector<double>>&& track)
: track_{std::move(track)},
  i_{0}
{}

TrackElementVector::~TrackElementVector() = default;

TrackElementExtended TrackElementVector::read(
    const std::optional<TrackElementExtended>& predecessor,
    const TransformationMatrix<double, double, 3>& inverse_geographic_mapping,
    size_t ntransformations)
{
    if (i_ == track_.size()) {
        ++i_;
        return TrackElementExtended{};
    }
    if (i_ > track_.size()) {
        throw std::runtime_error("Attempt to read past the end of the track");
    }
    return TrackElementExtended::create(
        predecessor,
        TrackElement::from_vector(track_[i_++], inverse_geographic_mapping, ntransformations));
}

bool TrackElementVector::eof() const {
    return i_ > track_.size();
}

void TrackElementVector::restart() {
    i_ = 0;
}
