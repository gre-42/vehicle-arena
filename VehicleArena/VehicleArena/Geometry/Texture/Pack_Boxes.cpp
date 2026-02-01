// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Pack_Boxes.hpp"

using namespace VA;

using NamedSize = std::pair<const VariableAndHash<std::string>*, const FixedArray<int, 2>*>;

std::list<std::list<NameAndBoxPosition>> VA::pack_boxes(
    const std::unordered_map<VariableAndHash<std::string>, FixedArray<int, 2>>& box_sizes,
    const FixedArray<int, 2>& container_size)
{
    if (box_sizes.empty()) {
        return {};
    }
    std::list<NamedSize> sorted;
    for (const auto& [n, b] : box_sizes) {
        sorted.push_back({&n, &b});
    }
    sorted.sort([](const NamedSize& a, const NamedSize& b){return (*a.second)(1) > (*b.second)(1);});
    std::list<std::list<NameAndBoxPosition>> result;
    result.emplace_back();
    int left = 0;
    int bottom = 0;
    int height = (*sorted.front().second)(1);
    while (!sorted.empty()) {
        const auto& [name_ptr, size_ptr] = sorted.front();
        const auto& name = *name_ptr;
        const auto& size = *size_ptr;
        if (size(0) > container_size(0)) {
            throw std::runtime_error("Width of box is too large");
        }
        if (size(1) > container_size(1)) {
            throw std::runtime_error("Height of box is too large");
        }
        if (left + size(0) > container_size(0)) {
            left = 0;
            bottom += height;
            height = size(1);
            if (bottom + size(1) > container_size(1)) {
                bottom = 0;
                result.emplace_back();
            }
        }
        result.back().emplace_back(NameAndBoxPosition{
            .name = name,
            .bottom_left = {left, bottom}});
        left += size(0);
        sorted.pop_front();
    }
    return result;
}
