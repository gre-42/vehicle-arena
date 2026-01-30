// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <boost/xpressive/xpressive_dynamic.hpp>
#include <string>

#define DECLARE_REGEX(name, value) decltype(boost::xpressive::cregex::compile(value)) name = boost::xpressive::cregex::compile(value)

namespace VA {
    namespace re {
        inline bool regex_search(const std::string& s, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_search(s.data(), s.data() + s.size(), regex);
        }
        inline bool regex_search(const char* begin, const char* end, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_search(begin, end, regex);
        }
        inline bool regex_search(const std::string& s, boost::xpressive::cmatch& match, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_search(s.data(), s.data() + s.size(), match, regex);
        }
        inline bool regex_search(const char* begin, const char* end, boost::xpressive::cmatch& match, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_search(begin, end, match, regex);
        }

        inline bool regex_match(const std::string& s, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_match(s.data(), s.data() + s.size(), regex);
        }
        inline bool regex_match(const char* begin, const char* end, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_match(begin, end, regex);
        }
        inline bool regex_match(const std::string& s, boost::xpressive::cmatch& match, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_match(s.data(), s.data() + s.size(), match, regex);
        }
        inline bool regex_match(const char* begin, const char* end, boost::xpressive::cmatch& match, const boost::xpressive::cregex& regex) {
            return boost::xpressive::regex_match(begin, end, match, regex);
        }
        inline auto cregex_token_iterator(const std::string& s, const boost::xpressive::cregex& regex) {
            return boost::xpressive::cregex_token_iterator(s.data(), s.data() + s.size(), regex, -1, boost::xpressive::regex_constants::match_not_null);
        }
        inline auto cregex_token_iterator() {
            return boost::xpressive::cregex_token_iterator();
        }
        using cregex = boost::xpressive::cregex;
        using cmatch = boost::xpressive::cmatch;
    }
    inline boost::xpressive::cregex compile_regex(const std::string& s) {
        return boost::xpressive::cregex::compile(s);
    }
}
