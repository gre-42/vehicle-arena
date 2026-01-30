// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Get_Thread_Name.hpp"
#include <stdexcept>
#include <stdexcept>

#ifdef __linux__

#include <linux/prctl.h>    // PR_* constants
#include <sys/prctl.h>
#include <cerrno>
#include <cstring>          // strerror

std::string VA::get_thread_name() {
    char buf[16];
    if (prctl(PR_GET_NAME, buf) != 0) {
        throw std::runtime_error(std::string("Could not get thread name: ") + strerror(errno));
    }
    return { buf };
}

#else

#include <Windows.h>
#include <processthreadsapi.h>

using namespace VA;

std::string VA::get_thread_name() {
    PWSTR data;
    auto hr = GetThreadDescription(GetCurrentThread(), &data);
    if (FAILED(hr)) {
        throw std::runtime_error("Could not get thread name");
    }
    std::string result(wcslen(data), '?');
    for (size_t i = 0; i < result.size(); ++i) {
        auto c = (char)data[i];
        if (c == data[i]) {
            result[i] = c;
        }
    }
    LocalFree(data);
    return result;
}

#endif
