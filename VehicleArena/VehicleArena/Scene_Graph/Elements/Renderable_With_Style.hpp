// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Scene_Graph/Elements/Color_Style.hpp>
#include <VehicleArena/Threads/Safe_Atomic_Shared_Mutex.hpp>
#include <memory>
#include <optional>

namespace VA {

class Renderable;

class RenderableWithStyle {
public:
    explicit RenderableWithStyle(std::shared_ptr<const Renderable> renderable);
    ~RenderableWithStyle();
    inline const Renderable* operator -> () const {
        return renderable_.get();
    }
    inline operator const Renderable& () const {
        return *renderable_.get();
    }
    inline const Renderable* get() const {
        return renderable_.get();
    }
    const ColorStyle* style(
        const std::list<std::shared_ptr<const ColorStyle>>& color_styles,
        const VariableAndHash<std::string>& name) const;
private:
    std::shared_ptr<const Renderable> renderable_;
    mutable SafeAtomicSharedMutex style_hash_mutex_;
    mutable std::optional<ColorStyle> style_;
    mutable size_t style_hash_;
};

}
