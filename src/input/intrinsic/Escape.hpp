﻿#pragma once

#include "../inputBase.hpp"
#include "../sources/boolInput.hpp"

namespace IntrinsicInput {
    using namespace InputHandler;
    void escapeCB(const std::multimap<EventPriority, BoolInput::BIEvent>& events, EventPriority priority, EventFlags flags, BoolInput::BoolInputID triggerId);
}