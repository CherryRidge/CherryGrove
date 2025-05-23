﻿#pragma once
#include <ctime>
#include <string>
#include <filesystem>

namespace Save {
    struct WorldInfo {
        std::string name;
        time_t lastModifiedTime;
        time_t createdTime;
        std::filesystem::path location;
        uint32_t savedVersion;
    };
}