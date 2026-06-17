#pragma once

#include "../../util/NameAndSpace.hpp"
#include "TCType.hpp"

#include "TCSchemaDef.hpp"

namespace World::TC {
    using Util::NameAndSpace;

    template <typename IntrinComp = void>
    struct TCDescriptor {
        TCType type;
        NameAndSpace nameAndSpace;
        TCSchemaDef<IntrinComp> runtimeTCSchema;
    };
}