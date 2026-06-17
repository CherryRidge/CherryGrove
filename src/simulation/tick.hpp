#pragma once
#include <atomic>
#include <thread>

#include "../util/time.hpp"
#include "../world/loading/loadingZones.hpp"
#include "currentTick.hpp"

namespace Simulation::internal {
    typedef uint64_t u64;
    using std::atomic, std::memory_order_release, std::this_thread::sleep_for;

    inline void tick() noexcept {
        sleep_for(Util::asMs(10));
        const auto sources = World::LoadingZone::detectLoadingSources();
        const auto chunkCoords = World::LoadingZone::getChunkCoordsInLoadingZones(sources);
        //Somehow figure out how to split the chunks to different working threads (the near-perfect-but-a-little-slow-so-run-it-every-few-seconds one). For now we just run everything in the simulation thread.
        




        currentTick.fetch_add(1, memory_order_release);
    //1. Update global state
    //2. Figure out all chunks that need to be simulated
    //3. Update thread pool with current state, and hand chunks to them
    //4. Wait for all threads to finish and yield transactions
    //5. Somehow detect and resolve conflicts betweeen txs
    //6. Signal all threads to apply txs besides the cancelled ones
    //Routine checks
    //i. Save to disk
    //ii. Unload 
    }
}