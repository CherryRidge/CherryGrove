#pragma once
#include <string>
#include <string_view>
#include <boost/container_hash/hash.hpp>

#include "../util/implHashFor.hpp"
#include "../util/lexical.hpp"

namespace Util {
    typedef uint64_t u64;
    using std::string, std::string_view;

    //There is only one invalid state: `colonPos == npos`. Everything else is valid.
    struct NameAndSpace {
    private:
        string data;
        u64 colonPos{string::npos};

        void check() noexcept {
            colonPos = data.find(':');
            if (colonPos != string::npos) {
                const string_view nameSpace = string_view(data.begin(), data.begin() + colonPos);
                const string_view identifier = string_view(data.begin() + colonPos + 1, data.end());
                if (!isValidIdentifier(nameSpace) || !isValidIdentifier(identifier)) colonPos = string::npos;
            }
        }

    public:
        [[nodiscard]] NameAndSpace() noexcept = default;
        [[nodiscard]] explicit NameAndSpace(const string_view raw) noexcept { update(raw); }
        [[nodiscard]] NameAndSpace(const string_view nameSpace, const string_view identifier) noexcept {
            data = string(nameSpace) + ":" + string(identifier);
            if (isValidIdentifier(nameSpace) && isValidIdentifier(identifier)) colonPos = nameSpace.size();
            else colonPos = string::npos;
        }

        [[nodiscard]] bool isValid() const noexcept { return colonPos != string::npos; }
        operator bool() const noexcept { return isValid(); }

        void update(const string_view raw) noexcept {
            data = string(raw);
            check();
        }

        [[nodiscard]] const string_view getRaw() const noexcept { return data; }
        [[nodiscard]] u64 getColonPos() const noexcept { return colonPos; }
        [[nodiscard]] const string_view getName() const noexcept {
            if (colonPos == string::npos) return "";
            else return string_view(data.begin(), data.begin() + colonPos);
        }
        [[nodiscard]] const string_view getNameSpace() const noexcept {
            if (colonPos == string::npos) return "";
            else return string_view(data.begin() + colonPos + 1, data.end());
        }

        bool operator==(const NameAndSpace& other) const noexcept { return data == other.data && colonPos == other.colonPos; }
    };
}

IMPL_HASH_FOR(Util, NameAndSpace, 555'555'555,
    boost::hash_combine(seed, input.getRaw());
    boost::hash_combine(seed, input.getColonPos());
)