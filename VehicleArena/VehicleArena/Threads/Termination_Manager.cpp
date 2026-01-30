// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Termination_Manager.hpp"
#include <VehicleArena/Os/Os.hpp>
#include <list>
#include <mutex>
#include <shared_mutex>

using namespace VA;

static std::list<std::exception_ptr> unhandled_exceptions;
static std::shared_mutex mutex;

void VA::add_unhandled_exception(std::exception_ptr ptr) {
    std::scoped_lock lock{mutex};
    unhandled_exceptions.push_back(ptr);
}

bool VA::unhandled_exceptions_occured() {
    std::shared_lock lock{mutex};
    return !unhandled_exceptions.empty();
}

void VA::print_unhandled_exceptions() {
    std::shared_lock lock{mutex};
    if (!unhandled_exceptions.empty()) {
        lerr() << unhandled_exceptions.size() << " unhandled exception(s)";
        for (const auto& e : unhandled_exceptions) {
            try {
                std::rethrow_exception(e);
            } catch (const std::exception& ex) {
                lerr() << "Unhandled exception: " << ex.what();
            } catch (...) {
                lerr() << "Unhandled exception of unknown type";
            }
        }
    }
}

void VA::print_unhandled_exceptions(std::ostream& ostr) {
    std::shared_lock lock{mutex};
    if (!unhandled_exceptions.empty()) {
        ostr << unhandled_exceptions.size() << " unhandled exception(s)" << std::endl;
        for (const auto& e : unhandled_exceptions) {
            try {
                std::rethrow_exception(e);
            } catch (const std::exception& ex) {
                ostr << "Unhandled exception: " << ex.what() << std::endl;
            } catch (...) {
                ostr << "Unhandled exception of unknown type" << std::endl;
            }
        }
    }
}
