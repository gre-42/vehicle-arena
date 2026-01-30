// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Thread_Safe_String.hpp"
#include <mutex>
#include <stdexcept>

using namespace VA;

ThreadSafeString::ThreadSafeString() = default;

ThreadSafeString::ThreadSafeString(const ThreadSafeString& other) {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    str_ = other.str_;
}

ThreadSafeString::ThreadSafeString(ThreadSafeString&& other) noexcept {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    str_ = std::move(other.str_);
}

ThreadSafeString& ThreadSafeString::operator = (const ThreadSafeString& other) {
    *this = (std::string)other;
    return *this;
}

ThreadSafeString& ThreadSafeString::operator = (std::string other) {
    std::scoped_lock lock{ mutex_ };
    str_ = std::move(other);
    return *this;
}

bool ThreadSafeString::operator == (const ThreadSafeString& other) const {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    return str_ == other.str_;
}

bool ThreadSafeString::operator != (const ThreadSafeString& other) const {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    return str_ != other.str_;
}

std::strong_ordering ThreadSafeString::operator <=> (const ThreadSafeString& other) const {
    std::scoped_lock lock{ mutex_, other.mutex_ };
    return str_ <=> other.str_;
}

ThreadSafeString::operator std::string() const {
    std::scoped_lock lock{ mutex_ };
    return str_;
}
