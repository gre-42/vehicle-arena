// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Memory/Dangling_Base_Class.hpp>
#include <VehicleArena/Object.hpp>
#include <VehicleArena/Os/Os.hpp>
#include <VehicleArena/Source_Location.hpp>
#include <VehicleArena/Threads/Fast_Mutex.hpp>
#include <compare>
#include <functional>
#include <memory>
#include <mutex>
#include <set>
#include <type_traits>
#include <unordered_set>

#ifdef _MSC_VER
#ifdef MlibMemory_EXPORTS
#define MLIB_MEMORY_API __declspec(dllexport)
#else
#define MLIB_MEMORY_API __declspec(dllimport)
#endif
#else
#define MLIB_MEMORY_API
#endif

namespace VA {

class ObjectPool;
    
struct ObjectAndSourceLocation {
    std::function<void()> deallocate;
    Object* object;
    SourceLocation loc;
    inline std::strong_ordering operator <=> (const ObjectAndSourceLocation& other) const {
        return object <=> other.object;
    }
};

enum class InObjectPoolDestructor {
    CLEAR,
    ASSERT_NO_LEAKS
};

template <class T>
class DeleteFromPool {
public:
    DeleteFromPool() noexcept;
    DeleteFromPool(std::nullptr_t) noexcept;
    DeleteFromPool(const DeleteFromPool& other) noexcept;
    explicit DeleteFromPool(ObjectPool& p) noexcept;
    void operator () (T* v);
private:
    ObjectPool* p_;
};

class ObjectPool: public virtual DanglingBaseClass {
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator = (const ObjectPool&) = delete;
public:
    template <class T>
    using UniquePtr = std::unique_ptr<T, DeleteFromPool<T>>;

    ObjectPool(InObjectPoolDestructor what_to_do_in_dtor);
    ~ObjectPool();

    template<class T, class... Args>
        requires std::is_convertible_v<T&, Object&>
    T& create(SourceLocation loc, Args&&... args) {
        T* o = std::allocator<T>().allocate(1);
        try {
            new (o) T(std::forward<Args>(args)...);
        } catch (...) {
            std::allocator<T>().deallocate(o, 1);
            throw;
        }
        add([o](){ std::allocator<T>().deallocate(o, 1); }, *o, loc);
        return *o;
    }
    template<class T>
        requires std::is_convertible_v<T&, Object&>
    T& add(std::unique_ptr<T>&& u, SourceLocation loc) {
        if (u == nullptr) {
            verbose_abort("Attempt to add nullptr to object pool");
        }
        auto o = u.release();
        add([o]() { std::allocator<T>().deallocate(o, 1); }, *o, loc);
        return *o;
    }
    template<class T, class... Args>
        requires std::is_convertible_v<T&, Object&>
    UniquePtr<T> create_unique(SourceLocation loc, Args&&... args) {
        auto& res = create<T>(loc, std::forward<Args>(args)...);
        return { &res, DeleteFromPool<T>(*this) };
    }
    template<class T, class... Args>
        requires std::is_convertible_v<T&, Object&>
    std::shared_ptr<T> create_shared(SourceLocation loc, Args&&... args) {
        auto& res = create<T>(loc, std::forward<Args>(args)...);
        return { &res, DeleteFromPool<T>(*this) };
    }
    template<class T>
        requires std::is_convertible_v<T&, Object&>
    std::unique_ptr<T> extract(UniquePtr<T>&& u) {
        if (u == nullptr) {
            verbose_abort("Attempt to extract a nullptr from the object pool");
        }
        extract(*u);
        return std::unique_ptr<T>{u.release()};
    }

    bool contains(Object* o) const;
    bool contains(Object& o) const;
    void remove(Object* o);
    void remove(Object& o);
    void extract(Object* o);
    void extract(Object& o);
    void clear();
    void assert_no_leaks() const;
private:
    void add(std::function<void()> deallocate, Object& o, SourceLocation loc);
    void delete_(const ObjectAndSourceLocation& o);
    mutable FastMutex mutex_;
    InObjectPoolDestructor what_to_do_in_dtor_;
    std::set<ObjectAndSourceLocation> ptrs_;
    std::unordered_set<Object*> deleting_ptrs_;
    bool clearing_;
};

template <class T>
DeleteFromPool<T>::DeleteFromPool() noexcept
    : p_{ nullptr }
{}

template <class T>
DeleteFromPool<T>::DeleteFromPool(std::nullptr_t) noexcept
    : p_{ nullptr }
{}

template <class T>
DeleteFromPool<T>::DeleteFromPool(const DeleteFromPool& other) noexcept
    : p_{ other.p_ }
{}

template <class T>
DeleteFromPool<T>::DeleteFromPool(ObjectPool& p) noexcept
    : p_{ &p }
{}

template <class T>
void DeleteFromPool<T>::operator () (T* v) {
    p_->remove(v);
}

MLIB_MEMORY_API extern ObjectPool global_object_pool;

};
