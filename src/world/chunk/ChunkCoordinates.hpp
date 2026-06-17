#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace World::Chunk {
    typedef int32_t i32;

    struct ChunkCoordinates {
        i32 x, y, z;

        [[nodiscard]] inline glm::vec<3, i32> toVec() const noexcept {
            return glm::vec<3, i32>{x, y, z};
        }

        bool operator==(const ChunkCoordinates& other) const noexcept {
            return x == other.x && y == other.y && z == other.z;
        }
    };
}