#pragma once
#include <cstdint>

namespace Tags {
    typedef uint8_t u8;

    //CGID: `:block_render_method`
    struct BlockRenderMethod {
        enum Type : u8 {
            Opaque, AlphaTest, Translucent
        } type{Opaque};
    };
}