#pragma once
#include <thread>
#include <vector>
#include <utility>
#include <flecs.h>
#include <glm/glm.hpp>

#include "../util/Promise.hpp"

namespace Simulation {
    typedef int32_t i32;
    typedef uint32_t u32;
    using std::thread, std::vector, std::pair, Util::Promise;

    struct ProposedTransaction {
        //This invalidates every single tick.
        u32 chunkIndex;
    };

    struct WorkerThread {
    private:
        thread threadObj;
        //This invalidates every single tick.
        pair<u32, u32> chunkIndiceRange;
        vector<ProposedTransaction> proposedTransactions;

    public:
        //Fuck constructors. Vectors are out of control on abuse calling them. We are using `initialize` instead.
        WorkerThread() = default;

        void initialize(pair<u32, u32> chunkIndiceRange) noexcept {
            this->chunkIndiceRange = chunkIndiceRange;
            this->threadObj = thread();
        }

        void phase1_aka_runTheUpdateAlgo_askContentPacksForUpdates_andProduce_a__proposedTransactions__() noexcept {

        }

        void phase3_aka_commitChangesToCentralStageSoWeCanCommitToFlecs() noexcept {

        }
    };


}