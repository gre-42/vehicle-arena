// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#include "Renderable_With_Style.hpp"
#include <VehicleArena/Hashing/Hash.hpp>
#include <shared_mutex>

using namespace VA;

static const size_t SEED = 0xc0febabe + 1;

RenderableWithStyle::RenderableWithStyle(std::shared_ptr<const Renderable> renderable)
    : renderable_{ std::move(renderable) }
    , style_hash_{ SEED }
{ }

RenderableWithStyle::~RenderableWithStyle() = default;

const ColorStyle* RenderableWithStyle::style(
    const std::list<std::shared_ptr<const ColorStyle>>& color_styles,
    const VariableAndHash<std::string>& name) const
{
    Hasher hasher0{ SEED };
    for (const auto& style : color_styles) {
        if (style->matches(name)) {
            hasher0.combine(style->get_hash());
        }
    }
    if (hasher0 == SEED) {
        return nullptr;
    }
    {
        std::shared_lock lock{ style_hash_mutex_ };
        if (hasher0 == style_hash_) {
            if (!style_.has_value()) {
                throw std::runtime_error("Style hash collision (0)");
            }
            return &*style_;
        }
    }
    {
        std::scoped_lock lock{ style_hash_mutex_ };
        if (hasher0 == style_hash_) {
            if (!style_.has_value()) {
                throw std::runtime_error("Style hash collision (1)");
            }
            return &*style_;
        }
        style_.reset();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
        style_.emplace();
#pragma GCC diagnostic pop
        Hasher hasher1{ SEED };
        for (const auto& style : color_styles) {
            if (style->matches(name)) {
                std::scoped_lock lock{
                    style->hash_mutex_,
                    style->parameter_mutex_};
                hasher1.combine(style->hash_.get());
                style_->insert(*style);
            }
        }
        style_hash_ = hasher1;
    }
    return &*style_;
}
