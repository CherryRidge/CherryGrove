#pragma once
#include <cstdint>

typedef uint64_t u64;

//Use `seed` to reference to the seed.
#define IMPL_HASH_FOR(ns, type, _seed, ...) \
namespace ns { \
    [[nodiscard]] inline u64 hash_value(const type& input) noexcept { \
        u64 seed = _seed; \
        __VA_ARGS__ \
        return seed; \
    } \
} \
namespace std { \
    template <> struct hash<ns::type> { \
        [[nodiscard]] u64 operator()(const ns::type& input) const noexcept { \
            return ns::hash_value(input); \
        } \
    }; \
}