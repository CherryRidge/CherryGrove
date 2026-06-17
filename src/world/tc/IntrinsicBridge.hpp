#pragma once

namespace World::TC {

    template <typename IntrinsicComponentType>
    struct IsFixedSize {
        [[nodiscard]] static constexpr bool isFixedSize() noexcept;
    };
    
    #define DECL_ISFIXEDSIZE(type, isFixedSize) \
    template <> struct IsFixedSize<type> { \
        [[nodiscard]] static constexpr bool value = isFixedSize; \
    };
}