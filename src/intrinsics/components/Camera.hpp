#pragma once
#include <glm/glm.hpp>

namespace Components {

    //CGID: `:camera`
    struct Camera {
        float fov{60.0f}, nearPlane{0.1f}, farPlane{100.0f};
        //The camera's position relative to the entity it's attached to.
        //I just don't believe someone will place the camera so far from the entity that float precision will be an issue. Device capabilities will be the limiting factor long before that happens.
        glm::vec3 deltaPosition{0.0f, 0.0f, 0.0f};
        glm::vec2 deltaRotation{0.0f, 0.0f}; //pitch, yaw
    };
}