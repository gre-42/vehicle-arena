// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <cstddef>
#include <stdexcept>

namespace VA {

template <class TImage>
void meshgrid(TImage& image, size_t axis) {
    typedef typename TImage::value_type TValue;
    if (image.ndim() == 1) {
        if (axis > 0) {
            throw std::runtime_error("Axis out of bounds");
        }
        for (size_t i = 0; i < image.length(); ++i) {
            image(i) = (TValue)i;
        }
    } else if (image.ndim() == 2) {
        if (axis > 1) {
            throw std::runtime_error("Axis out of bounds");
        }
        size_t i[2];
        for (i[0] = 0; i[0] < image.shape(0); ++i[0]) {
            for (i[1] = 0; i[1] < image.shape(1); ++i[1]) {
                image(i[0], i[1]) = (TValue)i[axis];
            }
        }
    } else {
        throw std::runtime_error("Unsupported meshgrid");
    }
}

}
