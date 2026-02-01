// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Env.hpp"
#include <VehicleArena/Strings/To_Number.hpp>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

using namespace VA;

static std::string g_app_reldir;

std::optional<std::string> VA::try_getenv(const char* name) {
    const char* result = getenv(name);
    if (result == nullptr) {
        return std::nullopt;
    } else {
        return result;
    }
}

std::string VA::str_getenv(const char* name) {
    auto res = try_getenv(name);
    if (res.has_value()) {
        return *res;
    } else {
        throw std::runtime_error("No environment variable with name \"" + std::string{ name } + "\" exists");
    }
}

const char* VA::getenv_default(const char* name, const char* deflt) {
    const char* result = getenv(name);
    if (result == nullptr) {
        return deflt;
    } else {
        return result;
    }
}

float VA::getenv_default_float(const char* n, float deflt) {
    const char* v = getenv(n);
    if (v == nullptr) {
        return deflt;
    }
    return VA::safe_stof(v);
}

int VA::getenv_default_int(const char* n, int deflt) {
    const char* v = getenv(n);
    if (v == nullptr) {
        return deflt;
    }
    return VA::safe_stoi(v);
}

unsigned int VA::getenv_default_uint(const char* n, unsigned int deflt) {
    const char* v = getenv(n);
    if (v == nullptr) {
        return deflt;
    }
    return VA::safe_stou(v);
}

size_t VA::getenv_default_size_t(const char* n, size_t deflt) {
    const char* v = getenv(n);
    if (v == nullptr) {
        return deflt;
    }
    return VA::safe_stoz(v);
}

bool VA::getenv_default_bool(const char* n, bool deflt) {
    const char* v = getenv(n);
    if (v == nullptr) {
        return deflt;
    }
    return VA::safe_stob(v);
}

#ifdef __ANDROID__
std::string VA::get_appdata_directory() {
    return "/";
}
#else
void VA::set_app_reldir(const std::string& app_reldir) {
    if (!g_app_reldir.empty()) {
        throw std::runtime_error("App reldir already set");
    }
    if (app_reldir.empty()) {
        throw std::runtime_error("Trying to set empty app reldir");
    }
    g_app_reldir = app_reldir;
}

std::string VA::get_appdata_directory() {
    if (g_app_reldir.empty()) {
        throw std::runtime_error("set_app_reldir not called before get_appdata_directory");
    }
#if defined(__linux__) || defined(__APPLE__)
    const char* parent = getenv("HOME");
#elif _WIN32
    const char* parent = getenv("APPDATA");
#else
    #error Could not determine OS
#endif
    if (parent == nullptr) {
        throw std::runtime_error("Could not determine home directory");
    }
    return (fs::path{parent} / fs::path{g_app_reldir}).string();
}
#endif

std::string VA::get_path_in_appdata_directory(const std::initializer_list<std::string>& child_path) {
    fs::path res = get_appdata_directory();
    for (const auto& s : child_path) {
        res /= fs::path(s);
    }
    return res.string();
}
