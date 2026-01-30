// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Regex/Regex_Select.hpp>
#include <VehicleArena/Regex/Template_Regex.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <functional>
#include <iosfwd>
#include <list>
#include <map>
#include <string>
#include <string_view>

namespace VA {

static const std::string substitute_pattern = "(?:\\S+:\\S*)?(?:\\s+\\S+:\\S*)*";

std::string substitute(
    const std::string& str,
    const std::map<std::string, std::string>& replacements);

std::string substitute_dollar(
    const std::string_view& str,
    const std::function<std::string(std::string_view)>& replacements);

std::map<std::string, std::string> replacements_to_map(const std::string& replacements);

void find_all(
    const std::string& str,
    const VA::re::cregex& pattern,
    const std::function<void(const VA::re::cmatch&)>& f);

template <class TRegex>
std::string_view find_all_templated(
    const std::string_view& str,
    const TRegex& pattern,
    const std::function<void(const TemplateRegex::SMatch<TRegex::ngroups + 1>&)>& f)
{
    TemplateRegex::SMatch<TRegex::ngroups + 1> match;
    std::string_view search_start = str;
    while (TemplateRegex::regex_match(search_start, match, pattern)) {
        f(match);
        search_start = search_start.substr(search_start.length() - match.suffix().length());
    }
    return search_start;
}

std::list<std::pair<std::string, std::string>> find_all_name_values(
    const std::string& str,
    const std::string& name_pattern,
    const std::string& value_pattern);

class SubstitutionMap {
    friend std::ostream& operator << (std::ostream& ostr, const SubstitutionMap& s);
public:
    SubstitutionMap();
    SubstitutionMap(const SubstitutionMap& other);
    SubstitutionMap& operator = (SubstitutionMap&& other);
    explicit SubstitutionMap(const std::map<std::string, std::string>& s);
    explicit SubstitutionMap(std::map<std::string, std::string>&& s);
    std::string substitute(const std::string& t) const;
    void merge(const SubstitutionMap& other, const std::string& prefix = "");
    bool insert(const std::string& key, const std::string& value);
    void set(const std::string& key, const std::string& value);
    void clear();
    const std::string& get_value(const std::string& key) const;
    bool get_bool(const std::string& key) const;
private:
    std::map<std::string, std::string> s_;
    mutable SafeAtomicRecursiveSharedMutex mutex_;
};

std::ostream& operator << (std::ostream& ostr, const SubstitutionMap& s);

}
