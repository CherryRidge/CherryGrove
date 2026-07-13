#pragma once
#include <stdexcept>
#include <vector>
#include <boost/unordered/unordered_flat_map.hpp>

#include "../../debug/loggers.hpp"
#include "../../util/RuntimeBijection.hpp"

namespace World::Record {
    typedef uint8_t u8;
    typedef uint64_t u64;
    using std::invalid_argument, std::vector, boost::unordered_flat_map, Util::RuntimeBijection;

    enum struct RecordType : u8 {
        Record,
        Int8, Int16, Int32, Int64,
        Uint8, Uint16, Uint32, Uint64,
        Float, Double,
        Bool,
        Raw8, Raw16, Raw32, Raw64,
        Array, //Dynamic ones. Use plain field combination for fixed-size ones.
        String,

        Count
    };

    struct RecordSchema {
        using enum RecordType;

        RecordType type;
        union {
            struct Record {
                vector<RecordSchema*> schema;
            } record;
            struct Array {
                RecordSchema* elementType;
            } array;
        };

        RecordSchema() noexcept : type(Count) {}
        template <RecordType type>
        [[nodiscard]] RecordSchema() noexcept : type(type) {}
        [[nodiscard]] RecordSchema(const vector<RecordSchema*>& schema) noexcept : type(Record), record{schema} {}
        [[nodiscard]] RecordSchema(RecordSchema* elementType) noexcept : type(Array), array{elementType} {}

        void destroyUnion() noexcept {
            switch (type) {
            case Record:
                record.schema.~vector();
                break;
            default:
                break;
            }
        }

        //Caution: This function only calculates the **fixed-size** part of the record. For dynamic parts, you need to detect them and handle accordingly.
        [[nodiscard]] u64 size() const noexcept {
            switch (type) {
                case Record: {
                    u64 sizeSum = 0;
                    for (u64 i = 0; i < record.schema.size(); i++) sizeSum += record.schema[i]->size();
                    return sizeSum;
                }
                case Int8: return 1;
                case Int16: return 2;
                case Int32: return 4;
                case Int64: return 8;
                case Uint8: return 1;
                case Uint16: return 2;
                case Uint32: return 4;
                case Uint64: return 8;
                case Float: return 4;
                case Double: return 8;
                case Bool: return 1;
                case Raw8: return 1;
                case Raw16: return 2;
                case Raw32: return 4;
                case Raw64: return 8;
                case Array:
                case String:
                    return 0;
                case Count:
                    lout << "`RecordSchema::size_fixedPart()` called on a `RecordSchema` with type `Count`." << nlaf;
                    return 0;
            }
        }
    };

    struct RecordInstance {
        vector<u8> data;
        vector<RecordSchema*> schemas;
        unordered_flat_map<u64, u64> dynFieldIds;
        vector<u64> dynFieldSizes;

        [[nodiscard]] RecordInstance(const vector<RecordSchema*>& schemas) noexcept : schemas(schemas) {
            u64 sizeSum = 0;
            for (u64 i = 0; i < schemas.size(); i++) {
                sizeSum += schemas[i]->size();
                if (!schemas[i]->isFixedSize()) {
                    dynFieldIds.emplace(i, dynFieldSizes.size());

                }
            }
        }


    };
}