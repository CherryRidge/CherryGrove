#pragma once
#include <cstdint>

namespace Components {
    typedef uint8_t u8;

    //CGID: `:light_level`
    struct LightLevel {
        u8 value{0};
    };
}