// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Json/Json_Key.hpp>
#include <VehicleArena/Macro_Executor/Json_Macro_Arguments.hpp>
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Threads/Recursive_Shared_Mutex.hpp>
#include <atomic>
#include <cstdint>
#include <functional>
#include <mutex>

namespace VA {

class JsonMacroArgumentsAndLock;
class WritableJsonMacroArgumentsAndLock;

class NotifyingJsonMacroArguments;

class JsonMacroArgumentsObserverToken {
    friend NotifyingJsonMacroArguments;
    JsonMacroArgumentsObserverToken(const JsonMacroArgumentsObserverToken&) = delete;
    JsonMacroArgumentsObserverToken& operator = (const JsonMacroArgumentsObserverToken&) = delete;
private:
    inline explicit JsonMacroArgumentsObserverToken(std::function<void()> cleanup);
public:
    ~JsonMacroArgumentsObserverToken();
private:
    std::function<void()> cleanup_;
};

class NotifyingJsonMacroArguments: public virtual DanglingBaseClass {
    friend JsonMacroArgumentsObserverToken;
    friend JsonMacroArgumentsAndLock;
    friend WritableJsonMacroArgumentsAndLock;
    NotifyingJsonMacroArguments(const NotifyingJsonMacroArguments&) = delete;
    NotifyingJsonMacroArguments& operator = (const NotifyingJsonMacroArguments&) = delete;
public:
    NotifyingJsonMacroArguments();
    ~NotifyingJsonMacroArguments();
    void set_and_notify(const std::string_view& key, const nlohmann::json& value);
    void set_and_notify(const std::vector<std::string>& key, const nlohmann::json& value);
    void merge_and_notify(const JsonView& other);
    void assign_and_notify(const JsonView& other);
    bool contains(const std::string_view& key) const;
    bool contains(const std::vector<std::string>& key) const;
    inline nlohmann::json at(const std::string_view& key) const {
        std::shared_lock lock{json_mutex_};
        return json_macro_arguments_.at(key);
    }
    template <class TResult>
    TResult at(const std::string_view& key) const {
        std::shared_lock lock{json_mutex_};
        return json_macro_arguments_.at<TResult>(key);
    }
    template <class TResult>
    TResult at(const std::string_view& key, const TResult& default_) const {
        std::shared_lock lock{json_mutex_};
        return json_macro_arguments_.at<TResult>(key, default_);
    }
    JsonMacroArgumentsAndLock json_macro_arguments() const;
    WritableJsonMacroArgumentsAndLock writable_json_macro_arguments();
    JsonMacroArgumentsObserverToken add_observer(std::function<void()> func);
    JsonMacroArgumentsObserverToken add_finalizer(std::function<void()> func);
private:
    template <JsonKey Key>
    void set_and_notify_generic(const Key& key, const nlohmann::json& value);
    void remove_observer(const std::list<std::function<void()>>::iterator& it);
    void remove_finalizer(const std::list<std::function<void()>>::iterator& it);
    mutable SafeAtomicRecursiveSharedMutex json_mutex_;
    mutable SafeAtomicRecursiveSharedMutex observer_mutex_;
    JsonMacroArguments json_macro_arguments_;
    std::list<std::function<void()>> observers_;
    std::list<std::function<void()>> finalizers_;
    std::atomic_uint32_t notification_counter_;
};

class JsonMacroArgumentsAndLock {
public:
    explicit JsonMacroArgumentsAndLock(const NotifyingJsonMacroArguments& args);
    ~JsonMacroArgumentsAndLock();
    const JsonMacroArguments* operator -> () const;
    operator const JsonMacroArguments&() const;
    operator const nlohmann::json&() const;
    void unlock();
private:
    std::shared_lock<SafeAtomicRecursiveSharedMutex> lock_;
    const NotifyingJsonMacroArguments& args_;
};

class WritableJsonMacroArgumentsAndLock {
public:
    explicit WritableJsonMacroArgumentsAndLock(NotifyingJsonMacroArguments& args);
    ~WritableJsonMacroArgumentsAndLock();
    JsonMacroArguments* operator -> ();
    operator JsonMacroArguments&();
    void unlock_and_notify();
private:
    std::unique_lock<SafeAtomicRecursiveSharedMutex> lock_;
    NotifyingJsonMacroArguments& args_;
};

}
