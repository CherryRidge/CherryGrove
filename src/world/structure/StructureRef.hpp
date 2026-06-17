#pragma once
#include <boost/uuid.hpp>

namespace World::Structure {
    using boost::uuids::uuid;

    struct StructureRef {
        uuid id;
    };
}