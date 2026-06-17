#pragma once
#include <stdexcept>

#include "../block/BlockRef.hpp"
#include "../dimension/DimensionRef.hpp"
#include "../entity/EntityRef.hpp"
#include "../locality/LocalityRef.hpp"
#include "../structure/StructureRef.hpp"
#include "GameObjectKind.hpp"

namespace World::Object {
    typedef uint8_t u8;
    using std::invalid_argument;

    struct GameObjectRef {
        GameObjectKind kind;
        union {
            Block::BlockRef block;
            Entity::EntityRef entity;
            Locality::LocalityRef locality;
            Structure::StructureRef structure;
            Dimension::DimensionRef dimension;
        };

        [[nodiscard]] explicit GameObjectRef(const GameObjectKind& kind) noexcept {
            if (kind.kind != GameObjectKindKind::World) throw std::invalid_argument("`GameObjectRef` must be constructed with an instanced reference if the kind is not `World`.");
        }

        #define INIT(type_, var, enum_) [[nodiscard]] GameObjectRef(const GameObjectKind& kind, const type_& var) noexcept : kind(kind), var(var) {}

        INIT(Block::BlockRef, block, Block)
        INIT(Entity::EntityRef, entity, Entity)
        INIT(Locality::LocalityRef, locality, Locality)
        INIT(Structure::StructureRef, structure, Structure)
        INIT(Dimension::DimensionRef, dimension, Dimension)

        #undef INIT

        void destroyUnion() noexcept {
            switch (type) {
                case Block:
                    block.~BlockRef();
                    break;
                case Entity:
                    entity.~EntityRef();
                    break;
                case Locality:
                    locality.~LocalityRef();
                    break;
                case Structure:
                    structure.~StructureRef();
                    break;
                case Dimension:
                    dimension.~DimensionRef();
                    break;
                case World:
                    break;
                case Count:
                    break;
            }
        }

        GameObject& operator=(const GameObject& other) noexcept {
            destroyUnion();
            switch (other.type) {
                case Block:
                    new (&block) Block::BlockRef(other.block);
                    break;
                case Entity:
                    new (&entity) Entity::EntityRef(other.entity);
                    break;
                case Locality:
                    new (&locality) Locality::LocalityRef(other.locality);
                    break;
                case Structure:
                    new (&structure) Structure::StructureRef(other.structure);
                    break;
                case Dimension:
                    new (&dimension) Dimension::DimensionRef(other.dimension);
                    break;
                case World:
                    break;
                case Count:
                    break;
            }
            type = other.type;
            return *this;
        }

        [[nodiscard]] GameObject(const GameObject& other) noexcept : type(Count) { operator=(other); }

        ~GameObject() { destroyUnion(); }
    };
}