#pragma once
#include <vector>
#include <utility>
#include <boost/unordered/unordered_flat_map.hpp>

#include "SlotTable.hpp"

namespace Util {
    typedef uint32_t u32;
    using std::vector, std::pair, boost::unordered::unordered_flat_map;

    //warning: This implementation stores `Left` and `Right` keys **twice**. Don't use this for large, owning objects like `string` or `vector`. Vector them and use their `u64` index.
    template <typename L, typename R>
    struct RuntimeBijection {
    private:
        MAKE_DISTINCT_HANDLE(Handle)
        SlotTable<pair<L, R>, Handle> storage;
        unordered_flat_map<L, Handle> ltr;
        unordered_flat_map<R, Handle> rtl;

    public:
        [[nodiscard]] RuntimeBijection() noexcept = default;
        [[nodiscard]] RuntimeBijection(const RuntimeBijection&) noexcept = default;
        [[nodiscard]] RuntimeBijection(RuntimeBijection&&) noexcept = default;
        RuntimeBijection& operator=(const RuntimeBijection&) noexcept = default;
        RuntimeBijection& operator=(RuntimeBijection&&) noexcept = default;

        void existsL(L&& left) noexcept {
            return ltr.contains(left);
        }

        void existsR(R&& right) noexcept {
            return rtl.contains(right);
        }

        void insert(L&& left, R&& right) noexcept {
            if (existsL(left)) {
                const auto handle = ltr.at(left);
                auto* entry = storage.get(handle);
                rtl.erase(rtl.find(entry->second));
                rtl.emplace(right, handle);
                entry->second = right;
            }
            else if (existsR(right)) {
                const auto handle = rtl.at(right);
                auto* entry = storage.get(handle);
                ltr.erase(ltr.find(entry->first));
                ltr.emplace(left, handle);
                entry->first = left;
            }
            else {
                const auto handle = storage.emplace(left, right);
                ltr.emplace(left, handle);
                rtl.emplace(right, handle);
            }
        }

        [[nodiscard]] bool findL(R&& right, L& left) noexcept {
            if (!existsR(right)) return false;
            const auto handle = rtl.at(right);
            const auto* entry = storage.get(handle);
            left = entry->first;
            return true;
        }

        [[nodiscard]] bool findR(L&& left, R& right) noexcept {
            if (!existsL(left)) return false;
            const auto handle = ltr.at(left);
            const auto* entry = storage.get(handle);
            right = entry->second;
            return true;
        }

        [[nodiscard]] bool eraseL(L&& left) noexcept {
            if (!existsL(left)) return false;
            const auto handle = ltr.at(left);
            const auto* entry = storage.get(handle);
            rtl.erase(rtl.find(entry->second));
            ltr.erase(ltr.find(left));
            static_cast<void>(storage.destroy(handle));
            return true;
        }

        [[nodiscard]] bool eraseR(R&& right) noexcept {
            if (!existsR(right)) return false;
            const auto handle = rtl.at(right);
            const auto* entry = storage.get(handle);
            ltr.erase(ltr.find(entry->first));
            rtl.erase(rtl.find(right));
            static_cast<void>(storage.destroy(handle));
            return true;
        }
    };
}