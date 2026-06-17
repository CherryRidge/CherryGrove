#pragma once
#include <boost/unordered/unordered_flat_map.hpp>

#include "BlockID.hpp"
#include "BlockDesc.hpp"

namespace World::Block {
    using boost::unordered_flat_map;

    namespace detail {
        inline BlockID nextId{0};
        inline unordered_flat_map<BlockID, BlockDesc> blockRegistry;
        inline unordered_flat_map<NameAndSpace, BlockID> nameToIdMap;
    }

    [[nodiscard]] inline BlockID registerBlock(const BlockDesc& desc) noexcept {
        const BlockID id = detail::nextId;
        detail::nextId++;
        detail::blockRegistry.emplace(id, desc);
        detail::nameToIdMap.emplace(desc.nameAndSpace, id);
        return id;
    }

    //note: There is no way to unregister a block. This is intentional.

    [[nodiscard]] inline bool getBlockDesc(BlockID id, BlockDesc& result) noexcept {
        const auto it = detail::blockRegistry.find(id);
        if (it != detail::blockRegistry.end()) {
            result = it->second;
            return true;
        }
        return false;
    }

    [[nodiscard]] inline bool getBlockDesc(const NameAndSpace& nameAndSpace, BlockDesc& result) noexcept {
        const auto it = detail::nameToIdMap.find(nameAndSpace);
        if (it != detail::nameToIdMap.end()) return getBlockDesc(it->second, result);
        return false;
    }

    inline void reset_blockRegistry() noexcept {
        detail::blockRegistry.clear();
        detail::nameToIdMap.clear();
    }
}