// !!!! WARNING !!!!!
// Please note that I cannot guarantee correctness and safety of the code, as SHA256 is not secure.
// echo jk | sha256sum: 720daff2aefd2b3457cbd597509b0fa399e258444302c2851f8d3cdd8ad781eb
// echo ks | sha256sum: 1aa44e718d5bc9b7ff2003dbbb6f154e16636d5c2128ffce4751af5124b65337
// echo xy | sha256sum: 3b2fc206fd92be3e70843a6d6d466b1f400383418b3c16f2f0af89981f1337f3
// echo za | sha256sum: 28832ea947ea9588ff3acbad546b27fd001a875215beccf0e5e4eee51cc81a2e

#pragma once
#include <VehicleArena/Physics/Misc/Inventory_Item.hpp>
#include <cstdint>
#include <map>
#include <string>

namespace VA {

struct ItemInfo {
    uint32_t capacity;
    uint32_t navailable;
};

class Inventory {
public:
    explicit Inventory();
    ~Inventory();
    bool knows_item_type(const InventoryItem& item_type) const;
    void set_capacity(const InventoryItem& item_type, uint32_t value);
    uint32_t navailable(const InventoryItem& item_type) const;
    uint32_t nfree(const InventoryItem& item_type) const;
    void add(const InventoryItem& item_type, uint32_t amount);
    void take(const InventoryItem& item_type, uint32_t amount);
private:
    const ItemInfo& item(const InventoryItem& item_type) const;
    ItemInfo& item(const InventoryItem& item_type);
    std::unordered_map<InventoryItem, ItemInfo> items_;
};

}
