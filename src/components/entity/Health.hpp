﻿#pragma once
#include <cstdint>

namespace Components {
    struct HealthComponent {
        uint32_t maxHealth;
        uint32_t health;
    };
}