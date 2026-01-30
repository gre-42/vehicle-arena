// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Extrapolate_Rgba_Colors.hpp"
#include <VehicleArena/Images/Filters/Gaussian_Filter.hpp>
#include <VehicleArena/Images/Normalize.hpp>
#include <VehicleArena/Images/StbImage4.hpp>
#include <VehicleArena/Misc/Floating_Point_Exceptions.hpp>

using namespace VA;

StbImage4 VA::extrapolate_rgba_colors(const StbImage4& img, float sigma, size_t niterations) {
    auto source = img.to_float_rgba();
    auto destination = source.copy();
    
    for (size_t i = 0; i < niterations; ++i) {
        Array<float> m =
            gaussian_filter_NWE(destination[3], sigma, float{NAN})
            .applied([](float v){return v == 0 ? float{NAN} : v;});
        for (size_t d = 0; d < 3; ++d) {
            TemporarilyIgnoreFloatingPointExeptions ignore_except;
            destination[d] = gaussian_filter_NWE(destination[d] * destination[3], sigma, float{NAN}) / m;

            auto d1 = destination[d].flattened();
            auto s1 = source[d].flattened();
            auto a1 = source[3].flattened();
            for (size_t j = 0; j < d1.length(); ++j) {
                if (a1(j) > 1e-1) {
                    d1(j) = s1(j);
                }
            }
        }
        destination[3] = m;
        if (!any(VA::isnan(m))) {
            break;
        }
    }
    destination[3] = source[3];
    
    return StbImage4::from_float_rgba(clipped(destination, 0.f, 1.f));
}
