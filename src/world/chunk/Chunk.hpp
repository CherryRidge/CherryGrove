#pragma once
#include <array>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>
#include <flecs.h>
#include <glm/glm.hpp>

#include "../block/BlockID.hpp"

namespace World::Chunk {
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint64_t u64;
    using std::array, std::vector, boost::unordered_flat_map, World::Block::BlockID;

    //Chunk size: 2**5 = 32, which will probably not change during the whole lifecycle of CherryGrove itself, but we have to be extra aware, so we don't hardcode it anywhere else.
    inline constexpr u8 CHUNK_SIZE_MAGNITUDE = 5;
    inline constexpr u64
        CHUNK_SIZE = 1ull << CHUNK_SIZE_MAGNITUDE,
        CHUNK_LAYER_BLOCK_COUNT = CHUNK_SIZE * CHUNK_SIZE,
        CHUNK_BLOCK_COUNT = CHUNK_LAYER_BLOCK_COUNT * CHUNK_SIZE;

    enum struct ChunkState : u8 {
        //Fully unloaded. The only purpose is to hold the chunk storage until the next save cycle to save it to the disk, so it can be dropped.
        Unloaded,
        //Nothing is loaded but the chunk allows worldgen content packs to modify the data, i.e. "generating terrain or something". The chunk is not guaranteed to be loaded ever, but it must be loaded in order to be saved. A generated but never loaded chunk will not be saved.
        Generating,
        //Nothing is loaded. The chunk is only for visual rendering. It will be dropped if not loaded and not seen for a while.
        VisualOnly,
        //Nothing is actively loaded but the chunk allows neighboring chunks to probe its state and write data to it. This is used as the outermost layer of the world for update consistency.
        Reactive,
        //Fully loaded. The chunk is actively simulated and rendered.
        Simulating
    };

    //todo: Block layer system (far future)
    struct Chunk {
        //Store the content of the chunk in a palette-based way to save memory.
        //This vector's `size()` can only be `0` (homogeneous chunk with all blocks being the default block `blockPalette[0]`), `CHUNK_BLOCK_COUNT` or `CHUNK_BLOCK_COUNT * 2` (when `blockPalette.size() > numeric_limits<u8>::max()`).
        //Uses x-y-z linear order.
        vector<u8> blockContent;
        //This needs to be GCed periodically so it don't grow indefinitely.
        vector<BlockID> blockPalette;
        //Every block that has at least one modified (non-default) component.
        //`u16` key is the linearized local coordinate of the block (x-y-z).
        unordered_flat_map<u16, flecs::entity_t> blockData;
        //Every coordinate grid that has at least one modified (non-default) component.
        //`u16` key is the linearized local coordinate of the grid (x-y-z).
        unordered_flat_map<u16, flecs::entity_t> localityData;
        u64 lastModifiedTick;
        ChunkState state{ChunkState::Unloaded};
    };
}