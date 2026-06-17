#pragma once
#include <array>
#include <glm/glm.hpp>

#include "../../graphics/texture/types.hpp"

namespace Tags {
    using std::array;

    //CGID: `:block_texture`
    //(x, y, z) relative to the block's position.
    struct BlockTexture {
        glm::vec3 start;
        glm::vec3 end;
        //(East, West, South, North, Up, Down)
        array<TexturePool::TextureHandle, 6> textures;
    };
}