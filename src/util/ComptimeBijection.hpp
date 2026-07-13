#pragma once
#include <array>

#include "concepts.hpp"

namespace Util {
    typedef uint64_t u64;
    using std::array, Util::UInt;
    
    template <typename T, u64 size, u64 sizeR> requires UInt<T>
    struct ComptimeBijection {
    private:
        using Arr = array<T, size>;
        using ArrR = array<T, sizeR>;
        Arr forward;
        ArrR reverse;

    public:
        //Don't mark explicit because we want syntax sugar of `ComptimeBijection arr = {{ ... }};`.
        [[nodiscard]] constexpr ComptimeBijection(const Arr& input, T invalid = 0, bool isDestinationInstead = false) : forward(input) {
            static_assert(sizeof(T) <= sizeof(u64), "Type T too large to be used as array index.");
            T maxv = 0;
            for (T i = 0; i < size; i++) if (input[i] > maxv) maxv = input[i];
            if (maxv + 1 > sizeR) throw "ComptimeBijection: sizeR too small for the provided input array.";
            forward.fill(invalid);
            reverse.fill(invalid);
            if (isDestinationInstead) for (T i = 0; i < size; i++) {
                reverse[i] = input[i];
                forward[input[i]] = i;
            }
            else for (T i = 0; i < size; i++) {
                forward[i] = input[i];
                reverse[input[i]] = i;
            }
        }
        [[nodiscard]] constexpr ComptimeBijection(const ComptimeBijection&) noexcept = default;
        [[nodiscard]] constexpr ComptimeBijection(ComptimeBijection&&) noexcept = default;
        constexpr ComptimeBijection& operator=(const ComptimeBijection&) noexcept = default;
        constexpr ComptimeBijection& operator=(ComptimeBijection&&) noexcept = default;

        [[nodiscard]] constexpr T to(T input) const noexcept { return forward[input]; }
        [[nodiscard]] constexpr T from(T input) const noexcept { return reverse[input]; }
    };
}