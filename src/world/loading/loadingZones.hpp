#pragma once
#include <cstdint>
#include <boost/unordered/unordered_flat_map.hpp>
#include <flecs.h>

#include "../../intrinsics/components/Coordinates.hpp"
#include "../../intrinsics/components/LoadingSource.hpp"
#include "../../util/CubeRange.hpp"
#include "../chunk/ChunkCoordinates.hpp"
#include "../world.hpp"
#include "LoadingZone.hpp"

namespace World::LoadingZone {
    typedef int32_t i32;
    using boost::unordered_flat_map, Util::CubeRange, Components::LoadingSource, Components::EntityCoordinates, Components::BlockCoordinates, World::Chunk::ChunkCoordinates;

    [[nodiscard]] inline vector<CubeRange> detectLoadingSources() noexcept {
        vector<CubeRange> result;
        flecs::query q = world.query<LoadingSource>();
        q.each([&result](flecs::entity e, const LoadingSource& data) noexcept {
            const auto [eCoords, bCoords] = e.try_get_n<EntityCoordinates, BlockCoordinates>();
            if (eCoords != nullptr) result.emplace_back(
                static_cast<i32>(eCoords->x) >> 4,
                static_cast<i32>(eCoords->y) >> 4,
                static_cast<i32>(eCoords->z) >> 4,
                data.radius
            );
            else if (bCoords != nullptr) result.emplace_back(
                bCoords->x >> 4,
                bCoords->y >> 4,
                bCoords->z >> 4,
                data.radius
            );
        });
        return result;
    }

    [[nodiscard]] inline vector<ChunkCoordinates> getChunkCoordsInLoadingZones(const vector<CubeRange>& zones) noexcept {
        vector<ChunkCoordinates> result;
        for (const auto& zone : zones) zone.getPoints(result);
        return result;
    }
}