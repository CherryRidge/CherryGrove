#pragma once

#include <flecs.h>
#include "world.hpp"

namespace World {

    struct Health {int health;};

    inline void a() {
        flecs::component healthComponent = world.component<Health>();
        healthComponent.set_name("Health");
        healthComponent.add(flecs::OnInstantiate, flecs::Inherit);
        flecs::entity itemPrototype = world.prefab().set<Health>({ .health = 5 });
        flecs::entity itemInstance = world.entity("item_instance_0").is_a(itemPrototype);
        const Health& inheritedHealth = itemInstance.get<Health>();
        const Health health = { .health = 3 };
        itemInstance.set<Health>({ .health = 1 });
        itemInstance.set_ptr(
            healthComponent,
            4,
            &health
        );
    }

    inline void initialize() noexcept {
        
    }

    inline void reset() noexcept {
        //todo: wire every reset to this.
    }
}