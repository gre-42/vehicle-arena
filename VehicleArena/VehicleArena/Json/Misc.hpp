// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Hashing/Variable_And_Hash.hpp>
#include <VehicleArena/Json/Base.hpp>
#include <VehicleArena/Math/Fixed_Point_Number.hpp>
#include <VehicleArena/Math/Orderable_Fixed_Array.hpp>
#include <map>
#include <string_view>
#include <unordered_map>

namespace VA {

// Workaround for issue https://github.com/nlohmann/json/issues/2932
template <class TKey, class TValue>
class junordered_map: public std::unordered_map<TKey, TValue> {};

template <class TValue>
void from_json(const nlohmann::json& j, junordered_map<VariableAndHash<std::string>, TValue>& v) {
    for (auto& [k, e] : j.get<std::map<std::string, TValue>>()) {
        v[VariableAndHash{ k }] = std::move(e);
    }
}

template <class TData, size_t tsize>
void to_json(nlohmann::json& j, const FixedArray<TData, tsize>& v) {
    j = std::vector(v.flat_begin(), v.flat_end());
}

template <class T>
T json_get(const nlohmann::json& j) {
    if constexpr (std::is_floating_point_v<T>) {
        if (j.type() == nlohmann::detail::value_t::string) {
            auto v = j.get<std::string>();
            if (v == "nan") {
                return NAN;
            } else if (v == "inf") {
                return INFINITY;
            } else if (v == "-inf") {
                return -INFINITY;
            } else {
                throw std::runtime_error("Unknown number string: \"" + v + '"');
            }
        }
    }
    return j.get<T>();
}

template <typename TData>
void from_json(const nlohmann::json& j, FixedArray<TData>& v) {
    v() = json_get<TData>(j);
}

template <typename TData, size_t tsize0, size_t... tshape>
void from_json(const nlohmann::json& j, FixedArray<TData, tsize0, tshape...>& v) {
    if (j.type() != nlohmann::detail::value_t::array) {
        throw std::runtime_error("JSON -> FixedArray received non-array type");
    }
    if (j.size() != tsize0) {
        throw std::runtime_error("JSON -> FixedArray received array of incorrect length");
    }
    for (size_t i = 0; i < tsize0; ++i) {
        v[i] = j[i].get<EFixedArray<TData, tshape...>>();
    }
}

template <typename TData, size_t... tshape>
void from_json(const nlohmann::json& j, OrderableFixedArray<TData, tshape...>& v) {
    v = j.get<EFixedArray<TData, tshape...>>();
}

template <class TData>
void from_json(const nlohmann::json& j, VariableAndHash<TData>& v) {
    v = json_get<TData>(j);
}

template <class TData>
void to_json(nlohmann::json& j, const VariableAndHash<TData>& v) {
    j = *v;
}

template <class TInt, std::intmax_t denominator>
void from_json(const nlohmann::json& j, FixedPointNumber<TInt, denominator>& v) {
    intermediate_float<TInt> vv;
    from_json(j, vv);
    v = FixedPointNumber<TInt, denominator>::from_float_safe(vv);
}

template <class TData, class TOperation>
auto get_vector(const nlohmann::json& j, const TOperation& op) {
    if (j.type() != nlohmann::detail::value_t::array) {
        throw std::runtime_error("JSON -> vector received non-array type");
    }
    std::vector<decltype(op(j.get<TData>()))> result;
    result.reserve(j.size());
    for (auto& e : j) {
        result.push_back(op(json_get<TData>(e)));
    }
    return result;
}

template <class TData, class TOperation>
auto get_vector_non_null(const nlohmann::json& j, const TOperation& op) {
    if (j.type() == nlohmann::detail::value_t::null) {
        return decltype(get_vector<TData>(j, op))();
    }
    return get_vector<TData>(j, op);
}

std::string get_multiline_string(const nlohmann::json& j);
void validate(const nlohmann::json& j, const std::set<std::string_view>& known_keys, std::string_view prefix = "");
void validate_complement(const nlohmann::json& j, const std::set<std::string_view>& known_keys, std::string_view prefix = "");

}
