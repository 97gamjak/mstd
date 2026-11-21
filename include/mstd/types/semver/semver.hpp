/*****************************************************************************
<GPL_HEADER>

    mstd library
    Copyright (C) 2025-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

#ifndef __MSTD__TYPES__SEMVER__SEMVER_HPP__
#define __MSTD__TYPES__SEMVER__SEMVER_HPP__

#include <compare>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>

namespace mstd
{
    /**
     * @brief Semantic version components (major, minor, patch).
     *
     * Totally ordered; lexicographic by major, minor, patch.
     */
    struct SemVerBase
    {
        size_t major;
        size_t minor;
        size_t patch;

        [[nodiscard]]
        constexpr auto operator<=>(const SemVerBase&) const = default;
        [[nodiscard]]
        constexpr bool operator==(const SemVerBase&) const = default;
    };

    /**
     * @brief A semantic version "major.minor.patch" with an optional
     * "+<hash>" extension (e.g. a git commit hash) and an optional 'v' prefix
     * that is fixed at compile time.
     *
     * The hash is an identifier, not an ordinal, so it never takes part in
     * ordering:
     *  - `==` is true iff base and hash are equal.
     *  - `<=>` yields `partial_ordering`: the base decides if it differs;
     *    with an equal base the result is `equivalent` for identical hashes
     *    and `unordered` otherwise (so <, >, <=, >= are all false).
     *  - To order versions regardless of their hash (e.g. for sorting or
     *    "at least version X" checks) compare `base()`.
     *
     * @tparam HasPrefix Whether the string form carries a leading 'v'.
     */
    template <bool HasPrefix>
    class SemVer
    {
        SemVerBase                 _base;
        std::optional<std::string> _hashExtension;

       public:
        static constexpr bool hasPrefix = HasPrefix;

        explicit SemVer(
            SemVerBase                 base,
            std::optional<std::string> hashExtension = std::nullopt
        );

        /**
         * @brief Strictly parses "[v]major.minor.patch[+hash]".
         *
         * @throws InvalidArgument on any deviation (wrong/missing prefix,
         * missing or non-numeric components, trailing characters, empty or
         * malformed hash).
         */
        [[nodiscard]]
        static SemVer from_string(std::string_view str);

        [[nodiscard]]
        std::string to_string() const;

        [[nodiscard]]
        const SemVerBase& base() const noexcept
        {
            return _base;
        }

        [[nodiscard]]
        const std::optional<std::string>& hash_extension() const noexcept
        {
            return _hashExtension;
        }

        [[nodiscard]]
        bool operator==(const SemVer& other) const = default;

        [[nodiscard]]
        std::partial_ordering operator<=>(const SemVer& other) const;
    };

    using SemVerWithPrefix    = SemVer<true>;
    using SemVerWithoutPrefix = SemVer<false>;

}   // namespace mstd

#ifndef __MSTD__TYPES__SEMVER__SEMVER_TPP__
#include "semver.tpp"   // IWYU pragma: export
#endif

#endif   // __MSTD__TYPES__SEMVER__VERSION_HPP__
