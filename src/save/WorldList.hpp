#pragma once
#include <ctime>
#include <filesystem>
#include <string>
#include <vector>
#include <nbt/nbt.hpp>

#include "../debug/loggers.hpp"
#include "../util/nbt/policy.hpp"
#include "../util/os/filesystem.hpp"
#include "WorldInfo/v1.hpp"

namespace Save {
    typedef uint8_t u8;
    typedef uint64_t u64;
    using std::time_t, std::move, std::string, std::vector, std::filesystem::exists, std::filesystem::is_directory, std::filesystem::create_directory, std::filesystem::directory_iterator, std::filesystem::path, Util::OS::readFile, Util::NBT::Latest_NBT, Util::NBT::NBTKind::WorldInfo;

    namespace detail {
        inline vector<Latest_NBT<WorldInfo>> worldList;
    }

    inline void refreshWorldList(const char* rootDir) noexcept {
        if (!exists(rootDir) || !is_directory(rootDir)) {
            create_directory(rootDir);
            return;
        }
        lout << "[WorldList] Loading saves!" << nlaf;
        vector<u8> fileData;
        NBTMap result;
        path dirPath, metaPath;
        for (const auto& directory : directory_iterator(rootDir)) {
            dirPath = directory.path();
            if (is_regular_file(dirPath)) continue;
            lout << "[WorldList] Found directory: " << dirPath << nlaf;
            if (!exists(metaPath) || !is_regular_file(metaPath)) continue;
            metaPath = dirPath / "world.cgb";
            if (!readFile(metaPath, fileData)) {
                lout << "[WorldList] Failed to read `world.cgb` in " << dirPath << nlaf;
                continue;
            }
            if (!NBT::readData<NBTPlc>(fileData, result)) {
                lout << "[WorldList] Failed to read `world.cgb` in " << dirPath << nlaf;
                continue;
            }
            const u64 fv = NBT::memberOr<Types::UVarInt, NBTPlc>(result, "formatVersion", 0);
            switch (fv) {
                case 1: {
                    const auto info = parse_v1(result);
                    lout << "[WorldList] Loaded world: " << info.name << nlaf;
                    detail::worldList.push_back(move(info));
                    break;
                }
                default:
                    lerr << "[WorldList] Unknown formatVersion " << fv << " in " << dirPath << nlaf;
                    break;
            }
        }
    }
}