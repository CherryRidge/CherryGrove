#pragma once
#include <flecs.h>

#include "../../util/NameAndSpace.hpp"
#include "GameObjectCategory.hpp"

namespace World::Object {
    using Util::NameAndSpace;

    struct GameObjectKind_HotData {
        flecs::entity prototype;
    };

    struct GameObjectKind_ColdData {
        NameAndSpace nameAndSpace;
        GameObjectCategory category;
    };
}