#pragma once
#include <flecs.h>

namespace World {
    inline flecs::world world;

    inline void reset_ECS() noexcept {
        world.reset();
    }
}