// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include <VehicleArena/Regex/Regex_Select.hpp>
#include <VehicleArena/Strings/String.hpp>
#include <list>
#include <stdexcept>

using namespace VA;

std::strong_ordering VA::operator <=> (const std::string& a, const std::string& b) {
    int i = a.compare(b);
    if (i < 0) {
        return std::strong_ordering::less;
    }
    if (i == 0) {
        return std::strong_ordering::equal;
    }
    return std::strong_ordering::greater;
}

std::list<std::string> VA::string_to_list(const std::string& str, const VA::re::cregex& re, size_t expected_length) {
    std::list<std::string> res;
    if (str.empty()) {
        return res;
    }
    for (auto it = VA::re::cregex_token_iterator(str, re);
        it != VA::re::cregex_token_iterator();
        ++it)
    {
        res.push_back(*it);
    }
    if ((expected_length != SIZE_MAX) && (res.size() != expected_length)) {
        throw std::runtime_error("Expected " + std::to_string(expected_length) + " elements, but got " + std::to_string(res.size()));
    }
    return res;
}

std::list<std::string> VA::string_to_list(const std::string& str, size_t expected_length) {
    static const DECLARE_REGEX(re, "\\s+");
    return string_to_list(str, re, expected_length);
}

std::vector<std::string> VA::string_to_vector(const std::string& str) {
    auto res = string_to_list(str);
    return std::vector<std::string>{res.begin(), res.end()};
}

std::set<std::string> VA::string_to_set(const std::string& str) {
    auto l = string_to_list(str);
    return {l.begin(), l.end()};
}

std::set<std::string> VA::string_to_set(const std::string& str, const VA::re::cregex& re, size_t expected_length) {
    auto l = string_to_list(str, re, expected_length);
    return {l.begin(), l.end()};
}
