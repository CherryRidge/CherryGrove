#pragma once

namespace Components {

    //CGID: `:basic_movement_props`
    struct BasicMovementProps {
        bool canWalk{true}, canFly{true}, canSwim{true};
        float walkSpeed{1.0f}, flySpeed{0.2f}, swimSpeed{0.5f};
    };
}