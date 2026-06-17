#pragma once
#include <memory>
#include <vector>

namespace World::TC {
    typedef uint8_t u8;
    typedef uint64_t u64;
    using std::unique_ptr, std::make_unique, std::move, std::vector;

    enum struct TCSchemaType : u8 {
        Schema,
        Int8, Int16, Int32, Int64,
        Uint8, Uint16, Uint32, Uint64,
        Float, Double, Bool,
        Raw8, Raw16, Raw32, Raw64,
        Index, Array, String,
        Count
    };

    struct TCSchemaDef {
        using enum TCSchemaType;

        TCSchemaType type;
        union {
            struct Schema {
                vector<TCSchemaDef> fields;
            } schema;
            struct Index {
                //The field index in this schema that this index specifically refers to, in `vector<TCSchemaDef> fields` of the parent schema.
                u64 ofFieldIdx;
            } index;
            struct Array {
                unique_ptr<TCSchemaDef> elementType;
            } array;
        };

        [[nodiscard]] TCSchemaDef() noexcept : type(Count) {}
        template <TCSchemaType type>
        [[nodiscard]] TCSchemaDef() noexcept : type(type) {
            static_assert(
                type != Schema
             && type != Index
             && type != Array,
                "Use the specific constructor for these types of schema."
            );
        }
        [[nodiscard]] TCSchemaDef(const vector<TCSchemaDef>& fields) noexcept : type(Schema) {
            new (&schema.fields) vector<TCSchemaDef>(fields);
        }
        [[nodiscard]] TCSchemaDef(u64 ofFieldIdx) noexcept : type(Index) {
            index.ofFieldIdx = ofFieldIdx;
        }
        [[nodiscard]] TCSchemaDef(unique_ptr<TCSchemaDef> elementType) noexcept : type(Array) {
            new (&array.elementType) unique_ptr<TCSchemaDef>(move(elementType));
        }

        void destroyUnion() noexcept {
            switch (type) {
                case Schema:
                    schema.fields.~vector();
                    break;
                case Array:
                    array.elementType.reset();
                    break;
                default:
                    break;
            }
        }

        TCSchemaDef& operator=(const TCSchemaDef& other) noexcept {
            if (this != &other) {
                destroyUnion();
                type = other.type;
                switch (type) {
                    case Schema:
                        new (&schema.fields) vector<TCSchemaDef>(other.schema.fields);
                        break;
                    case Array:
                        new (&array.elementType) unique_ptr<TCSchemaDef>(other.array.elementType.get());
                        break;
                    default:
                        break;
                }
            }
            return *this;
        }

        [[nodiscard]] TCSchemaDef(const TCSchemaDef& other) noexcept : type(Count) { operator=(other); }

        ~TCSchemaDef() noexcept {
            destroyUnion();
        }

        [[nodiscard]] inline bool isFixedSize() const noexcept {
            switch (type) {
                case Schema:
                    for (const auto& field : schema.fields) {
                        if (!field.isFixedSize()) return false;
                    }
                    return true;
                case Int8:
                case Int16:
                case Int32:
                case Int64:
                case Uint8:
                case Uint16:
                case Uint32:
                case Uint64:
                case Float:
                case Double:
                case Bool:
                case Raw8:
                case Raw16:
                case Raw32:
                case Raw64:
                case Index:
                    return true;
                case Array:
                case String:
                    return false;
                case Count:
                    return false;
            }
        }
    };
}