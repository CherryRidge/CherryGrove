#pragma once
#include <vector>

#include "RecordDesc.hpp"

namespace World::Record {
    using std::vector;

    namespace detail {
        inline vector<RecordDesc> registry;
    }

    
}