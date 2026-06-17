#pragma once
#include <cstdint>

namespace World::TC {
    typedef uint8_t u8;

    struct TCType {
        [[nodiscard]] TCType(bool isTag, bool isFixedSize, bool isIntrinsic, bool isReadonly) noexcept {
            flag |= static_cast<u8>(isTag) << 0;
            flag |= static_cast<u8>(isFixedSize) << 1;
            flag |= static_cast<u8>(isIntrinsic) << 2;
            flag |= static_cast<u8>(isReadonly) << 3;
        }

        [[nodiscard]] bool isTag() const noexcept { return (flag & (1 << 0)) != 0; }
        [[nodiscard]] bool isFixedSize() const noexcept { return (flag & (1 << 1)) != 0; }
        [[nodiscard]] bool isIntrinsic() const noexcept { return (flag & (1 << 2)) != 0; }
        [[nodiscard]] bool isReadonly() const noexcept { return (flag & (1 << 3)) != 0; }

    private:
        //Bit 0: Tag / Component.
        //Bit 1: Fixed-size / Variable-size (runtime components only, because intrinsic components are all fixed-size and known at compile time, if we need variable size, we will use `std::vector`).
        //Bit 2: Intrinsic / Runtime.
        //Bit 3: Readonly (derived or view intrinsic components, or tags) / Writable (normal components).
        u8 flag{0};
    };
}