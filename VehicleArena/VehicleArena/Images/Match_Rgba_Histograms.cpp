// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Match_Rgba_Histograms.hpp"
#include <VehicleArena/Math/Math.hpp>
#include <VehicleArena/Stats/Histogram_Matching.hpp>

using namespace VA;

Array<unsigned char> VA::match_rgba_histograms(
    const Array<unsigned char>& image,
    const Array<unsigned char>& ref,
    unsigned char alpha_threshold)
{
    Array<unsigned char> out{image.shape()};
    if (image.ndim() != 3) {
        throw std::runtime_error("Image does not have 3 dimensions");
    }
    if (ref.ndim() != 3) {
        throw std::runtime_error("Reference image does not have 3 dimensions");
    }
    if (image.shape(0) != 3 && image.shape(0) != 4) {
        throw std::runtime_error("Image does not have 3 or 4 channels");
    }
    if (ref.shape(0) != 3 && ref.shape(0) != 4) {
        throw std::runtime_error("Reference image does not have 3 or 4 channels");
    }
    if ((image.shape(0) == 3) && (ref.shape(0) == 3)) {
        for (size_t d = 0; d < 3; ++d) {
            out[d] = histogram_matching<unsigned char, unsigned char, float>(image[d].flattened(), ref[d].flattened(), 256);
        }
    } else {
        Array<bool> mask = (image.shape(0) == 3)
            ? ones<bool>(image.shape().erased_first())
            : (image[3] > alpha_threshold);
        Array<bool> mask_ref = (ref.shape(0) == 3)
            ? ones<bool>(ref.shape().erased_first())
            : (ref[3] > alpha_threshold);
        for (size_t d = 0; d < 3; ++d) {
            HistogramMatching<unsigned char, unsigned char, float> hm{image[d][mask], ref[d][mask_ref], 256};
            for (size_t r = 0; r < image.shape(1); ++r) {
                for (size_t c = 0; c < image.shape(2); ++c) {
                    out(d, r, c) = hm(image(d, r, c), true);
                }
            }
        }
        if (image.shape(0) == 4) {
            out[3] = image[3];
        }
    }
    out.reshape(image.shape());
    return out;
}
