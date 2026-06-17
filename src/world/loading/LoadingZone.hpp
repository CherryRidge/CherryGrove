#pragma once
#include <vector>

#include "../../util/CubeRange.hpp"
#include "../chunk/Chunk.hpp"

namespace World::LoadingZone {
    typedef uint16_t u16;
    using std::vector, Util::CubeRange, World::Chunk::Chunk;

    struct LoadingZoneDesc {
        u16 visRadius, simRadius;
    };

    struct LoadingZone {
        vector<CubeRange> subZones;
        vector<LoadingZoneDesc> subZoneDescs;
        //size() = (visRadius * 2 + 1) ** 3, just use cube loading zones now, don't want to overcomplicate things.
        vector<Chunk> chunks;
    };
}