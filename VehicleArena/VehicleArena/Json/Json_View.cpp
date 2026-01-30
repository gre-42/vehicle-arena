// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Json_View.hpp"
#include <ostream>

using namespace VA;

JsonView::JsonView(
    const nlohmann::json& j,
    CheckIsObjectBehavior check)
: j_{j}
{
    if ((check == CheckIsObjectBehavior::CHECK) &&
        (j_.type() != nlohmann::detail::value_t::object))
    {
        throw std::runtime_error("JSON is not of type object");
    }
}

bool JsonView::contains(std::string_view name) const {
    return j_.contains(name);
}

bool JsonView::contains_non_null(std::string_view name) const {
    return j_.contains(name) &&
           (j_.at(name).type() != nlohmann::detail::value_t::null);
}

static const nlohmann::json* get_internal(
    const nlohmann::json& j,
    const std::vector<std::string>& keys,
    size_t rec)
{
    if (keys.empty()) {
        throw std::runtime_error("JSON contains called with empty path");
    }
    if (j.type() != nlohmann::detail::value_t::object) {
        throw std::runtime_error("JSON entry is not an object");
    }
    auto it = j.find(keys[rec]);
    if (it == j.end()) {
        return nullptr;
    }
    if (rec == keys.size() - 1) {
        return &it.value();
    }
    return get_internal(*it, keys, rec + 1);
}

bool JsonView::contains(const std::vector<std::string>& name) const {
    return get_internal(j_, name, 0) != nullptr;
}

bool JsonView::contains_non_null(const std::vector<std::string>& name) const {
    auto v = get_internal(j_, name, 0);
    return ((v == nullptr) && (v->type() != nlohmann::detail::value_t::null));
}

std::optional<nlohmann::json> JsonView::try_at(const std::vector<std::string>& name) const {
    auto v = get_internal(j_, name, 0);
    if (v == nullptr) {
        return std::nullopt;
    }
    return *v;
}

std::optional<nlohmann::json> JsonView::try_at_non_null(const std::vector<std::string>& name) const {
    auto v = get_internal(j_, name, 0);
    if ((v == nullptr) || (v->type() == nlohmann::detail::value_t::null)) {
        return std::nullopt;
    }
    return *v;
}

std::optional<nlohmann::json> JsonView::try_resolve(std::string_view key) const {
    return try_at(key);
}

std::optional<nlohmann::json> JsonView::try_at(std::string_view name) const {
    if (!j_.contains(name)) {
        return std::nullopt;
    }
    return j_.at(name);
}

std::optional<nlohmann::json> JsonView::try_at_non_null(std::string_view name) const {
    auto it = j_.find(name);
    if ((it == j_.end()) || (it.value().type() == nlohmann::detail::value_t::null)) {
        return std::nullopt;
    }
    return *it;
}

nlohmann::json JsonView::at(std::string_view name) const {
    auto it = j_.find(name);
    if (it == j_.end()) {
        throw std::runtime_error("Cannot find key with name \"" + std::string{ name } + "\"");
    }
    return *it;
}

std::ostream& VA::operator << (std::ostream& ostr, const JsonView& view) {
    ostr << "JSON: " << view.j_;
    return ostr;
}
