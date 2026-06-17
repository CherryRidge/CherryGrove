#pragma once
#include <vector>

#include "Chunk.hpp"

namespace World::Chunk {
    using std::vector;

    inline vector<Chunk> loadedChunks;
}