#pragma once
#include <cstdlib>
#include <type_traits>
#include <vector>
#include <glm/glm.hpp>

#include "concepts.hpp"

namespace Util {
    typedef int32_t i32;
    typedef uint32_t u32;
    using std::abs, std::vector, std::move, std::remove_cvref_t;

    template <typename T>
    concept HasXYZ = requires(remove_cvref_t<T> t) {
        { move(t.x) } -> EqualStrict<i32&&>;
        { move(t.y) } -> EqualStrict<i32&&>;
        { move(t.z) } -> EqualStrict<i32&&>;
    };

    //Range at any dimension:
    //[origin - size, origin + size]
    //Volume:
    //(size * 2 + 1) ** 3
    struct CubeRange {
        i32 x, y, z;
        u32 size;

        [[nodiscard]] bool contains(const glm::vec<3, i32>& point) const noexcept {
            return point.x >= x - size && point.x <= x + size
                && point.y >= y - size && point.y <= y + size
                && point.z >= z - size && point.z <= z + size;
        }

        [[nodiscard]] bool intersects(const CubeRange& other) const noexcept {
            return other.x - other.size <= x + size && other.x + other.size >= x - size
                && other.y - other.size <= y + size && other.y + other.size >= y - size
                && other.z - other.size <= z + size && other.z + other.size >= z - size;
        }

        template <typename T> requires HasXYZ<T>
        void getPoints(vector<T>& result) const noexcept {
            for (i32 x_ = x - size; x_ <= x + size; x_++) {
                for (i32 y_ = y - size; y_ <= y + size; y_++) {
                    for (i32 z_ = z - size; z_ <= z + size; z_++) {
                        result.emplace_back(x_, y_, z_);
                    }
                }
            }
        }
    };
}