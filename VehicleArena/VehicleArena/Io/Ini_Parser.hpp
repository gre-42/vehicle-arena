// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Strings/To_Number.hpp>
#include <filesystem>
#include <iosfwd>
#include <list>
#include <map>
#include <optional>
#include <string>

namespace VA {

class IniParser {
public:
    explicit IniParser(const std::filesystem::path& filename);
    const std::string& get(const std::string& section, const std::string& key) const;
    template <class T>
    T get(const std::string& section, const std::string& key) const {
        return safe_stox<T>(get(section, key));
    }
    std::optional<std::string> try_get(const std::string& section, const std::string& key) const;
    template <class T>
    std::optional<T> try_get(const std::string& section, const std::string& key) const {
        auto s = try_get(section, key);
        if (!s.has_value()) {
            return std::nullopt;
        }
        return safe_stox<T>(*s);
    }
    const std::map<std::string, std::map<std::string, std::string>>& sections() const;
    const std::map<std::string, std::list<std::map<std::string, std::string>>>& lists() const;
    void print(std::ostream& ostr) const;
private:
    std::map<std::string, std::map<std::string, std::string>> sections_;
    std::map<std::string, std::list<std::map<std::string, std::string>>> lists_;
};

}
