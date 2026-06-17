#pragma once
#include <glm/glm.hpp>

namespace Tags {

    //CGID: `:collision`
    //(x, y, z) relative to the block's position.
    struct CollisionBox {
        glm::vec3 start;
        glm::vec3 end;
    };
}