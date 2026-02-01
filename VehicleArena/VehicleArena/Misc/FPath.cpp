// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "FPath.hpp"
#include <stdexcept>
#include <string_view>

using namespace VA;
using namespace std::literals;

static const auto LOCAL_PATH_PREFIX = "file://"sv;
static const auto VARIABLE_PREFIX = "$"sv;

FPath::FPath(const std::string_view& uri) {
    if (uri.starts_with(LOCAL_PATH_PREFIX)) {
        type_ = PathType::LOCAL_PATH;
        path_or_variable_ = uri.substr(LOCAL_PATH_PREFIX.length());
        return;
    }
    if (uri.starts_with(VARIABLE_PREFIX)) {
        type_ = PathType::VARIABLE;
        path_or_variable_ = uri.substr(VARIABLE_PREFIX.length());
        return;
    }
    throw std::runtime_error("Unkown URI type: \"" + std::string{uri} + '"');
}

FPath::~FPath() = default;

PathType FPath::type() const {
    return type_;
}

std::string FPath::local_path() const {
    if (type_ != PathType::LOCAL_PATH) {
        throw std::runtime_error("URI is not a local path: \"" + path_or_variable_ + '"');
    }
    return path_or_variable_;
}

std::string FPath::variable() const {
    if (type_ != PathType::VARIABLE) {
        throw std::runtime_error("URI is not a variable: \"" + path_or_variable_ + '"');
    }
    return path_or_variable_;
}
