#pragma once
#include <cstdint>
#include <cstdlib>
#include <glm/glm.hpp>

namespace Util {
    typedef int32_t i32;
    typedef uint32_t u32;
    using std::abs;

    //When `size` is 0, the range only contains the origin point. When `size` is 1, the range contains the origin point and the six points that are 1 block away from the origin point in the six axis directions, and so on.
    struct ManhattanRange {
        i32 x, y, z;
        u32 size;

        [[nodiscard]] bool contains(const glm::vec<3, i32>& point) const noexcept {
            return
               static_cast<u32>(abs(point.x - x))
             + static_cast<u32>(abs(point.y - y))
             + static_cast<u32>(abs(point.z - z))
             <= size;
        }

        //Is the point right on the inner side of the edge of the range? (i.e. is it in the range, but if we shrink the range by 1, it won't be in the range anymore?)
        [[nodiscard]] bool isEdgeInner(const glm::vec<3, i32>& point) const noexcept {
            return
               static_cast<u32>(abs(point.x - x))
             + static_cast<u32>(abs(point.y - y))
             + static_cast<u32>(abs(point.z - z))
             == size;
        }

        //Is the point right on the outer side of the edge of the range? (i.e. is it outside the range, but if we expand the range by 1, it will be in the range?)
        [[nodiscard]] bool isEdgeOuter(const glm::vec<3, i32>& point) const noexcept {
            return
               static_cast<u32>(abs(point.x - x))
             + static_cast<u32>(abs(point.y - y))
             + static_cast<u32>(abs(point.z - z))
             == size + 1;
        }

        [[nodiscard]] bool intersects(const ManhattanRange& other) const noexcept {
            return
               static_cast<u32>(abs(other.x - x))
             + static_cast<u32>(abs(other.y - y))
             + static_cast<u32>(abs(other.z - z))
             <= size + other.size;
        }
    };
}