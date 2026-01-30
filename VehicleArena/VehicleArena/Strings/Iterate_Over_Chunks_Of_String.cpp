// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Iterate_Over_Chunks_Of_String.hpp"

using namespace VA;

std::list<std::string_view> VA::iterate_over_blocks_of_string(
    std::string_view s,
    size_t block_size)
{
    std::list<std::string_view> result;
    for (size_t i = 0; i < s.length(); i += block_size) {
        result.push_back(s.substr(i, std::min(s.length(), i + block_size)));
    }
    return result;
}
