#pragma once
#include <flecs.h>

#include "../world.hpp"

namespace World::TC {

    template <typename... ComptimeComps>
    [[nodiscard]] inline flecs::entity registerPrototype() noexcept {
        
    }
}

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <span>
#include <type_traits>
#include <utility>

#include <flecs.h>

namespace cg {

using HashedCGID = std::uint64_t;

struct TcTypeHandle {
    std::uint32_t index;
};

enum class TcKind : std::uint8_t {
    Tag,
    Component,
};

enum class TcStoragePolicy : std::uint8_t {
    FlecsTyped,
    FlecsUntyped,
    CgDerived,
};

class TcRegistry {
public:
    [[nodiscard]] const TcDescriptor& getDescriptor(TcTypeHandle handle) const noexcept {
        // 这里换成你自己的 dense vector。
        return descriptors[handle.index];
    }

private:
    std::span<const TcDescriptor> descriptors;
};

// ---------- default spec wrappers ----------

template<class T>
struct StaticComponentDefault {
    using ComponentType = T;

    T value;
};

template<class T>
struct StaticTagDefault {
    using TagType = T;
};

struct RuntimeComponentDefault {
    TcTypeHandle typeHandle;
    std::span<const std::byte> valueBytes;
};

struct RuntimeTagDefault {
    TcTypeHandle typeHandle;
};

// ---------- helper constructors ----------

template<class T>
[[nodiscard]] constexpr auto staticComponentDefault(T&& value) noexcept {
    using ComponentType = std::remove_cvref_t<T>;

    return StaticComponentDefault<ComponentType> {
        .value = std::forward<T>(value),
    };
}

template<class T>
[[nodiscard]] constexpr auto staticTagDefault() noexcept {
    return StaticTagDefault<T> {};
}

[[nodiscard]] constexpr RuntimeComponentDefault runtimeComponentDefault(
    TcTypeHandle typeHandle,
    std::span<const std::byte> valueBytes
) noexcept {
    return RuntimeComponentDefault {
        .typeHandle = typeHandle,
        .valueBytes = valueBytes,
    };
}

[[nodiscard]] constexpr RuntimeTagDefault runtimeTagDefault(TcTypeHandle typeHandle) noexcept {
    return RuntimeTagDefault {
        .typeHandle = typeHandle,
    };
}

template<class T>
    requires std::is_trivially_copyable_v<T>
[[nodiscard]] std::span<const std::byte> bytesOf(const T& value) noexcept {
    return std::as_bytes(std::span<const T>(&value, 1));
}

// ---------- prototype registry ----------

class PrototypeRegistry {
public:
    PrototypeRegistry(
        flecs::world& flecsWorld,
        const TcRegistry& tcRegistry
    ) noexcept
        : flecsWorld(flecsWorld)
        , tcRegistry(tcRegistry) {}

    template<typename... DefaultSpecs>
    [[nodiscard]] inline flecs::entity registerPrototype(DefaultSpecs&&... defaultSpecs) noexcept {
        flecs::entity prototypeEntity = flecsWorld.entity().add(flecs::Prefab);

        (applyPrototypeDefault(
            prototypeEntity,
            std::forward<DefaultSpecs>(defaultSpecs)
        ), ...);

        return prototypeEntity;
    }

private:
    flecs::world& flecsWorld;
    const TcRegistry& tcRegistry;

    template<class T>
    inline void applyPrototypeDefault(
        flecs::entity& prototypeEntity,
        const StaticComponentDefault<T>& defaultSpec
    ) noexcept {
        // 编译期 component：走 flecs typed API。
        // 前提：T 已经在 CG 初始化阶段显式 registerStaticTc<T>()。
        prototypeEntity.set<T>(defaultSpec.value);
    }

    template<class T>
    inline void applyPrototypeDefault(
        flecs::entity& prototypeEntity,
        StaticTagDefault<T>
    ) noexcept {
        // 编译期 tag：走 flecs typed API。
        prototypeEntity.add<T>();
    }

    inline void applyPrototypeDefault(
        flecs::entity& prototypeEntity,
        RuntimeComponentDefault defaultSpec
    ) noexcept {
        const TcDescriptor& descriptor =
            tcRegistry.getDescriptor(defaultSpec.typeHandle);

        assert(descriptor.kind == TcKind::Component);
        assert(descriptor.storagePolicy == TcStoragePolicy::FlecsUntyped);
        assert(descriptor.flecsId != 0);
        assert(descriptor.size == defaultSpec.valueBytes.size());

        ecs_set_id(
            flecsWorld.c_ptr(),
            prototypeEntity.id(),
            descriptor.flecsId,
            descriptor.size,
            defaultSpec.valueBytes.data()
        );
    }

    inline void applyPrototypeDefault(
        flecs::entity& prototypeEntity,
        RuntimeTagDefault defaultSpec
    ) noexcept {
        const TcDescriptor& descriptor =
            tcRegistry.getDescriptor(defaultSpec.typeHandle);

        assert(descriptor.kind == TcKind::Tag);
        assert(descriptor.flecsId != 0);

        ecs_add_id(
            flecsWorld.c_ptr(),
            prototypeEntity.id(),
            descriptor.flecsId
        );
    }
};

} // namespace cg