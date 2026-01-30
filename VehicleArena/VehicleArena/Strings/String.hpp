// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <compare>
#include <list>
#include <set>
#include <string>
#include <vector>

#ifndef __clang__
#include <functional>
#include <ranges>
#endif

namespace VA {

std::strong_ordering operator <=> (const std::string& a, const std::string& b);
std::list<std::string> string_to_list(const std::string& str, const VA::re::cregex& re, size_t expected_length = SIZE_MAX);
std::list<std::string> string_to_list(const std::string& str, size_t expected_length = SIZE_MAX);
std::vector<std::string> string_to_vector(const std::string& str);
std::set<std::string> string_to_set(const std::string& str);
std::set<std::string> string_to_set(const std::string& str, const VA::re::cregex& re, size_t expected_length = SIZE_MAX);
#ifdef __clang__
inline const std::string& identity_(const std::string& v) {
    return v;
}
template <class TContainer, class TOperation = decltype(identity_)>
std::string join(
    const std::string& delimiter,
    const TContainer& lst,
    const TOperation& op = identity_)
#else
template <class TContainer, class TOperation = std::identity>
requires std::ranges::range<TContainer>
std::string join(const std::string& delimiter, const TContainer& lst, const TOperation& op = {})
#endif
{
    std::string res;
    int i = 0;
    for (const auto& s : lst) {
        res += (i++ == 0)
            ? op(s)
            : delimiter + op(s);
    }
    return res;
}

template <class TOperation>
auto string_to_vector(const std::string& str, const VA::re::cregex& re, const TOperation& op, size_t expected_length = SIZE_MAX) {
    std::list<std::string> sresult = string_to_list(str, re, expected_length);
    std::vector<decltype(op(""))> result;
    result.reserve(sresult.size());
    for (const std::string& s : sresult) {
        result.push_back(op(s));
    }
    return result;
}

template <class TOperation>
auto string_to_vector(const std::string& str, const TOperation& op, size_t expected_length = SIZE_MAX) {
    static const DECLARE_REGEX(re, "\\s+");
    return string_to_vector(str, re, op, expected_length);
}

}
