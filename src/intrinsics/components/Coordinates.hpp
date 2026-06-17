#pragma once
#include <cstdint>
#include <glm/glm.hpp>

namespace Components {
    typedef int32_t i32;
    typedef uint32_t u32;

    //CGID: `:block_coordinates`
    struct BlockCoordinates {
        i32 x{0}, y{0}, z{0};
        u32 dimension{0};

        bool operator==(const BlockCoordinates& other) const noexcept {
            return x == other.x && y == other.y && z == other.z && dimension == other.dimension;
        }
    };

    //CGID: `:entity_coordinates`
    struct EntityCoordinates {
        double x{0.0}, y{0.0}, z{0.0};
        u32 dimension{0};

        //Use runtime calculated relative coordinates instead of casting to inprecise floats first.
        [[nodiscard]] glm::vec3 temp_getGLMVec3() const noexcept { return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)}; }

        bool operator==(const EntityCoordinates& other) const noexcept {
            return x == other.x && y == other.y && z == other.z && dimension == other.dimension;
        }
    };
}