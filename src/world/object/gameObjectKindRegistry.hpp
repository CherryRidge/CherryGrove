#pragma once
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "../../util/NameAndSpace.hpp"
#include "GameObjectKind.hpp"

namespace World::Object {
    typedef uint64_t u64;
    using std::vector, boost::unordered_flat_map, Util::NameAndSpace;

    namespace detail {
        inline vector<GameObjectKind_HotData> kindRegistryHot;
        inline vector<GameObjectKind_ColdData> kindRegistryCold;
        inline unordered_flat_map<NameAndSpace, u64> reverseIndex;
    }

    struct GameObjectHandle {
        u64 index_DO_NOT_USE_ME_UNLESS_YOU_KNOW_WHAT_YOU_ARE_DOING__INSTEAD_PASS_ME_TO_THE_API;
    };

    [[nodiscard]] inline bool registerGameObjectKind(const GameObjectKind_HotData& hotData, const GameObjectKind_ColdData& coldData, GameObjectHandle& result) noexcept {
        if (detail::reverseIndex.contains(coldData.nameAndSpace)) return false;
        detail::kindRegistryHot.push_back(hotData);
        detail::kindRegistryCold.push_back(coldData);
        detail::reverseIndex.emplace(coldData.nameAndSpace, detail::kindRegistryHot.size() - 1);
        result.index_DO_NOT_USE_ME_UNLESS_YOU_KNOW_WHAT_YOU_ARE_DOING__INSTEAD_PASS_ME_TO_THE_API = detail::kindRegistryHot.size() - 1;
        return true;
    }

    inline void reset_gameObjectKindRegistry() noexcept {
        detail::kindRegistryHot.clear();
        detail::kindRegistryCold.clear();
        detail::reverseIndex.clear();
    }
}