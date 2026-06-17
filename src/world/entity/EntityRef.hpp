#pragma once
#include <boost/uuid.hpp>

namespace World::Entity {
    using boost::uuids::uuid;

    struct EntityRef {
        uuid id;
    };
}