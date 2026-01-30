// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#ifdef __SANITIZE_ADDRESS__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#include <regex>
#pragma GCC diagnostic pop
#else
#include <regex>
#endif
#include <string>

#define DECLARE_REGEX(name, value) decltype(std::regex{ value }) name = std::regex{ value }

namespace VA {
    namespace re {
        inline bool regex_search(const std::string& s, const std::regex& regex) {
            return std::regex_search(s.data(), s.data() + s.size(), regex);
        }
        inline bool regex_search(const char* begin, const char* end, const std::regex& regex) {
            return std::regex_search(begin, end, regex);
        }
        inline bool regex_search(const std::string& s, std::cmatch& match, const std::regex& regex) {
            return std::regex_search(s.data(), s.data() + s.size(), match, regex);
        }
        inline bool regex_search(const char* begin, const char* end, std::cmatch& match, const std::regex& regex) {
            return std::regex_search(begin, end, match, regex);
        }

        inline bool regex_match(const std::string& s, const std::regex& regex) {
            return std::regex_match(s.data(), s.data() + s.size(), regex);
        }
        inline bool regex_match(const char* begin, const char* end, const std::regex& regex) {
            return std::regex_match(begin, end, regex);
        }
        inline bool regex_match(const std::string& s, std::cmatch& match, const std::regex& regex) {
            return std::regex_match(s.data(), s.data() + s.size(), match, regex);
        }
        inline bool regex_match(const char* begin, const char* end, std::cmatch& match, const std::regex& regex) {
            return std::regex_match(begin, end, match, regex);
        }
        inline auto cregex_token_iterator(const std::string& s, const std::regex& regex) {
            return std::cregex_token_iterator(s.data(), s.data() + s.size(), regex, -1, std::regex_constants::match_not_null);
        }
        inline auto cregex_token_iterator() {
            return std::cregex_token_iterator();
        }
        using cregex = std::regex;
        using cmatch = std::cmatch;
    }
    inline std::regex compile_regex(const std::string& s) {
        return std::regex{ s };
    }
}
