#pragma once
#include <filesystem>
#include <string>
#include <boost/unordered/unordered_flat_map.hpp>

#include "../debug/Fatal.hpp"
#include "../debug/loggers.hpp"
#include "../settings/pack.hpp"
#include "../settings/Settings.hpp"
#include "../umi/controller.hpp"
#include "packFetcher.hpp"
#include "PackMetaInfo.hpp"
#include "registry.hpp"
#include "shadowing.hpp"

namespace Pack {
    typedef uint64_t u64;
    using std::filesystem::current_path, std::string, boost::unordered_flat_map, Util::Json::Latest, Util::Json::JSONKind::Settings;

    inline void updatePacks() noexcept {
        const Latest<Settings>::Packs& packSettings = Settings::getSettings().packs;
        for (u64 i = 0; i < packSettings.knownPacks.size(); i++) detail::knownPacks.emplace(packSettings.knownPacks[i].id.value(), packSettings.knownPacks[i]);

        //Get Packs from the default pack root `/packs` and any additional pack roots specified in settings. This is separate from the "additionalPacks" in settings, which are specific pack paths.
        detail::getPacksFromPackRoot("packs");
        for (u64 i = 0; i < packSettings.additionalPackRoots.size(); i++) detail::getPacksFromPackRoot(packSettings.additionalPackRoots[i]);

        //Get Packs from specific paths specified in settings.
        PackMetaInfo info;
        for (u64 i = 0; i < packSettings.additionalPacks.size(); i++) {
            if (detail::parsePackManifest(packSettings.additionalPacks[i], info)) detail::tryAddingPack(info);
            else lerr << "[Pack] Failed to parse pack: " << packSettings.additionalPacks[i] << nlaf;
        }
        lout << "[Pack] Found " << detail::packInfo.size() << " valid packs." << nlaf;

        if (!Settings::updateKnownPacks(detail::knownPacks)) {
            lerr << "[Pack] Failed to write new known packs to settings file." << nlaf;
            Debug::exit(Debug::SETTINGS_FAILED_TO_SAVE);
        }

        detail::updateShadowing();
    }

    inline void init() noexcept {
        Umi::init();
        updatePacks();
    }

    inline void shutdown() noexcept {
        Umi::shutdown();
    }
}