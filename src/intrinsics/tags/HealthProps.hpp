#pragma once
#include <cstdint>

namespace Tags {
    typedef uint32_t u32;

    //CGID: `:health_props`
    struct HealthProps {
        u32 maxHealth{0};
        bool isInvulnerable{false};
    };
}