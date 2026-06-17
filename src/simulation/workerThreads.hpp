#pragma once
#include <thread>
#include <vector>
#include <utility>
#include <boost/unordered/unordered_flat_map.hpp>
#include <glm/glm.hpp>

#include "../world/chunk/ChunkCoordinates.hpp"
#include "../world/chunk/loadedChunks.hpp"

namespace Simulation {
    typedef int32_t i32;
    typedef uint32_t u32;
    using std::thread, std::vector, std::pair, std::max, boost::unordered_flat_map, World::Chunk::ChunkCoordinates;

    namespace detail {
        struct WorkerThread {
            thread threadObj;
            ChunkCoordinates currentChunk;
        };

        inline vector<WorkerThread> workerThreads;

        //`chunkIndiceRange`: [)
        //`u32`: I promise we will never load more than 4 billion chunks at once.
        inline void work(pair<u32, u32> chunkIndiceRange) noexcept {
            
        }

        inline void initializeWorkerThread() noexcept {
            
        }
    }

    inline void initWorkerThreads() noexcept {
        /* //Danger! Concurrency ahead, use single thread for now
        detail::workerThreads.resize(max(thread::hardware_concurrency(), 1u));
        for (auto& workerThread : detail::workerThreads) {
            workerThread.threadObj = thread(detail::work);
        }*/
    }

    inline void signalPhase1() noexcept {
        //Currently we just work on the simulation thread.
        detail::work({0, World::Chunk::loadedChunks.size()});
    }
}