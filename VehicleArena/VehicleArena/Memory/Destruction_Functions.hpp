// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Misc/Source_Location.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <atomic>
#include <functional>
#include <list>
#include <map>

namespace VA {

class DestructionFunctions;

class DestructionFunctionsRemovalTokens {
    friend DestructionFunctions;
    DestructionFunctionsRemovalTokens(const DestructionFunctionsRemovalTokens&) = delete;
    DestructionFunctionsRemovalTokens& operator = (const DestructionFunctionsRemovalTokens&) = delete;
public:
    explicit DestructionFunctionsRemovalTokens(DestructionFunctions& funcs, SourceLocation loc);
    explicit DestructionFunctionsRemovalTokens(DestructionFunctions* funcs, SourceLocation loc);
    ~DestructionFunctionsRemovalTokens();
    void add(std::function<void()> f, SourceLocation loc);
    void clear();
    void set(DestructionFunctions& funcs, SourceLocation loc);
    void set(DestructionFunctions* funcs, SourceLocation loc);
    bool empty() const;
    bool is_null() const;
    inline const SourceLocation loc() const {
        return loc_;
    }
private:
    void clear_unsafe();
    mutable FastMutex mutex_;
    SourceLocation loc_;
    DestructionFunctions* funcs_;
};

struct FuncAndSourceLocation {
    std::function<void()> func;
    SourceLocation loc;
};

class DestructionFunctions {
    friend DestructionFunctionsRemovalTokens;
    DestructionFunctions(const DestructionFunctions&) = delete;
    DestructionFunctions& operator = (const DestructionFunctions&) = delete;
    using Funcs = std::map<DestructionFunctionsRemovalTokens*, std::list<FuncAndSourceLocation>>;
private:
    void add(
        DestructionFunctionsRemovalTokens& tokens,
        std::function<void()> f,
        SourceLocation loc);
    void remove(DestructionFunctionsRemovalTokens& tokens);
    Funcs funcs_;
    mutable FastMutex mutex_;
    std::atomic_bool clearing_;
public:
    DestructionFunctions();
    ~DestructionFunctions();
    void clear();
    bool empty() const;
    void print_source_locations() const;
};

}
