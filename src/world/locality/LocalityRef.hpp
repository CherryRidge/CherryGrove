#pragma once
#include "../../intrinsics/components/Coordinates.hpp"

namespace World::Locality {
    using Components::BlockCoordinates;

    struct LocalityRef {
        BlockCoordinates coords;
    };
}