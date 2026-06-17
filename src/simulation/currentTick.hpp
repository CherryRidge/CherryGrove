#pragma once
#include <atomic>

namespace Simulation::internal {
    typedef uint64_t u64;
    using std::atomic;

    inline atomic<u64> currentTick{0};
}