#pragma once
#include <cstdint>

namespace Components {
    typedef uint32_t u32;

    //CGID: `:health`
    struct Health {
        u32 health{0};
    };
}