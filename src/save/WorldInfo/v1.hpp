#pragma once
#include <ctime>
#include <filesystem>
#include <string>
#include <vector>
#include <nbt/nbt.hpp>

#include "../../util/nbt/latest.hpp"
#include "../../util/nbt/policy.hpp"

namespace Save {
    typedef uint8_t u8;
    typedef uint32_t u32;
    using std::filesystem::path, std::string, std::vector;

    struct WorldInfo_v1 {
        path folderPath;
        string name;
        time_t createdTime, lastModified;
        u32 engineVersion;
        vector<u8> thumbnail;
    };
}

REGISTER_LATEST_NBT(WorldInfo, Save::WorldInfo_v1);

namespace Save {
    typedef uint32_t u32;
    using NBT::memberOr, NBT::Types, Util::NBT::Latest_NBT, Util::NBT::NBTKind::WorldInfo;

    [[nodiscard]] inline WorldInfo_v1 parse_v1(const NBTMap& data) noexcept {
        return {
            .name = memberOr<Types::String, NBTPlc>(data, "name", "Unnamed World"),
            .createdTime = memberOr<Types::IVarInt, NBTPlc>(data, "createdTime", 0),
            .lastModified = memberOr<Types::IVarInt, NBTPlc>(data, "lastModifiedTime", 0),
            .engineVersion = static_cast<u32>(memberOr<Types::UVarInt, NBTPlc>(data, "engineVersion", 0)),
            .thumbnail = memberOr<Types::ArrayRaw, NBTPlc>(data, "thumbnail", {})
        };
    }
}