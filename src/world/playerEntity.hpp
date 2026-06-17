#pragma once
#include <flecs.h>

namespace World {
    inline flecs::entity playerEntity;

    inline void reset_playerEntity() noexcept {
        playerEntity = flecs::entity();
    }
}