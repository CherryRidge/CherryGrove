#pragma once
#include <cstdint>

#include "../../intrinsics/components/Coordinates.hpp"

namespace World::Block {
    typedef uint16_t u16;
    using Components::BlockCoordinates;

    struct BlockRef {
        BlockCoordinates coords;
        u16 layerIdx;
    };
}