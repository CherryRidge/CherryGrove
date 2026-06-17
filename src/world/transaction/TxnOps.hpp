#pragma once
#include <glm/glm.hpp>

#include "../../intrinsics/components/Coordinates.hpp"
#include "../chunk/ChunkCoordinates.hpp"

namespace World::Transaction {
    typedef int32_t i32;
    using World::Chunk::ChunkCoordinates;

    struct SetBlockOp {
        Components::BlockCoordinates coords;
        uint16_t blockId;
    };

    struct Component;
    struct Data;

    struct ModifyComponentOp {
        Components::BlockCoordinates coords;
        Component* component;
        Data* newData;
    };

    struct MoveBlockOp {
        ChunkCoordinates coordStart;
        ChunkCoordinates coordEnd;
        ChunkCoordinates delta;
    };

    struct SetEntityOp {
        
    };

    struct Operation {
        union {
            SetBlockOp setBlock;
            MoveBlockOp moveBlock;
            SetEntityOp setEntity;
        };


    };
}