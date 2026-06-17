#pragma once
#include <glm/glm.hpp>

namespace Tags {

    //CGID: `:selection`
    //(x, y, z) relative to the block's position (Caution! Not the sub block's position!).
    struct SelectionBox {
        glm::vec3 start;
        glm::vec3 end;
    };
}