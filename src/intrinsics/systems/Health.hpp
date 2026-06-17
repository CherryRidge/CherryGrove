#pragma once
#include <cstdint>
#include <flecs.h>

#include "../components/Health.hpp"

namespace Systems {
    typedef int32_t i32;
    typedef uint32_t u32;

    [[nodiscard]] inline bool updateHealth(flecs::entity entity,
        u32 newHealth
    ) noexcept {
        if (entity.has<Components::Health>()) {
            auto& health = entity.ensure<Components::Health>();
            health.health = newHealth;
            return true;
        }
        return false;
    }

    [[nodiscard]] inline bool deltaHealth(flecs::entity entity,
        i32 delta
    ) noexcept {
        if (entity.has<Components::Health>()) {
            auto& health = entity.ensure<Components::Health>();
            health.health += delta;
            return true;
        }
        return false;
    }
}