#pragma once
#include <cstdint>

namespace World::Object {
    typedef uint8_t u8;

    enum struct GameObjectCategory : u8 {
        Block, Entity, Structure, Locality, Dimension, World,
        Count
    };
}