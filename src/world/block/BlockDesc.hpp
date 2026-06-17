#pragma once
#include <vector>

#include "../../intrinsics/tags/BlockRenderMethod.hpp"
#include "../../intrinsics/tags/BlockTexture.hpp"
#include "../../intrinsics/tags/CollisionBox.hpp"
#include "../../intrinsics/tags/SelectionBox.hpp"
#include "../../util/NameAndSpace.hpp"

namespace World::Block {
    using std::vector, Util::NameAndSpace;

    struct BlockDesc {
        NameAndSpace nameAndSpace;
        Tags::BlockRenderMethod renderMethod;
        //todo: Generate meshes from these data and store the mesh instead of the data itself.
        vector<Tags::BlockTexture> textures;
        vector<Tags::CollisionBox> collisionBoxes;
        vector<Tags::SelectionBox> selectionBoxes;
    };
}