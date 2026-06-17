#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace Graphics {
    typedef uint64_t u64;
    using std::vector;

    struct RenderItem {
        u64 entityId;
        glm::vec<3, double> relPos;
        glm::vec<3, double> rotation;
        //For interpolation usages.
        glm::vec<3, double> velocity{0.0, 0.0, 0.0};
        glm::vec<3, double> scale{1.0, 1.0, 1.0};
    };

    using RendererBuffer = vector<RenderItem>;
}