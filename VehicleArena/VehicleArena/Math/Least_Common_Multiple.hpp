// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <vector>

namespace VA {

template <class TIterable>
auto least_common_multiple(
    const TIterable& begin,
    const TIterable& end,
    const std::remove_reference_t<decltype(*begin)>& tolerance,
    size_t max_iterations)
{
    std::vector<std::remove_reference_t<decltype(*begin)>> data0(begin, end);
    std::vector<std::remove_reference_t<decltype(*begin)>> dataI(begin, end);
    if (data0.empty()) {
        throw std::runtime_error("least_common_multiple received empty sequence");
    }
    for (size_t iteration = 0; iteration < max_iterations; ++iteration) {
        if (std::all_of(
            dataI.begin() + 1,
            dataI.end(),
            [&dataI, &tolerance](const auto& v) { return std::abs(v - dataI[0]) <= tolerance; }))
        {
            return dataI[0];
        }
        auto i = (size_t)(std::min_element(dataI.begin(), dataI.end()) - dataI.begin());
        dataI[i] += data0[i];
    }
    throw std::runtime_error("least_common_multiple did not terminate");
}

}
