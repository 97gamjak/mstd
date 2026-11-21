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

#ifndef __MSTD__TYPES__SEMVER__SEMVER_TPP__
#define __MSTD__TYPES__SEMVER__SEMVER_TPP__

#include "mstd/exceptions.hpp"
#include "semver.hpp"

namespace mstd::detail
{
    /**
     * @brief Parses an unsigned decimal number from the front of `str` and
     * removes it from `str`. Rejects empty input and overflow.
     */
    constexpr bool consume_number(std::string_view& str, size_t& out)
    {
        constexpr size_t max = static_cast<size_t>(-1);

        size_t value  = 0;
        size_t digits = 0;
        while (digits < str.size() && str[digits] >= '0' && str[digits] <= '9')
        {
            const size_t d = static_cast<size_t>(str[digits] - '0');
            if (value > (max - d) / 10)
                return false;   // overflow
            value = value * 10 + d;
            ++digits;
        }

        if (digits == 0)
            return false;

        out = value;
        str.remove_prefix(digits);
        return true;
    }

    /**
     * @brief Consumes a single character `c` from the front of `str` if
     * present.
     *
     * @param str The string view to consume from.
     * @param c The character to consume.
     * @return true if the character was present and consumed, false otherwise.
     */
    constexpr bool consume_char(std::string_view& str, char c)
    {
        if (str.empty() || str.front() != c)
            return false;

        str.remove_prefix(1);
        return true;
    }

    /**
     * @brief Hash extension: non-empty, [0-9A-Za-z.-] only (SemVer build
     * metadata alphabet).
     */
    constexpr bool is_valid_hash_extension(std::string_view str)
    {
        if (str.empty())
            return false;

        for (const char c : str)
        {
            const bool ok = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') ||
                            (c >= 'A' && c <= 'Z') || c == '.' || c == '-';
            if (!ok)
                return false;
        }
        return true;
    }
}   // namespace mstd::detail

namespace mstd
{
    /**
     * @brief Constructs a semantic version from its base and optional hash.
     *
     * @param base Major, minor and patch numbers.
     * @param hash Hash / build metadata without the '+'; empty for none.
     * @throws InvalidArgument if the hash is too long or malformed.
     */
    template <bool HasPrefix>
    constexpr SemVer<HasPrefix>::SemVer(SemVerBase base, std::string_view hash)
        : _base(base)
    {
        if (hash.empty())
            return;

        if (hash.size() > maxHashLength)
            throw InvalidArgument("Hash extension is too long");

        if (!detail::is_valid_hash_extension(hash))
            throw InvalidArgument("Malformed hash extension");

        for (size_t i = 0; i < hash.size(); ++i)
            _hash[i] = hash[i];
        _hashLength = hash.size();
    }

    /**
     * @brief Strictly parses "[v]major.minor.patch[+hash]".
     *
     * @throws InvalidArgument on any deviation (wrong/missing prefix,
     * missing or non-numeric components, trailing characters, empty,
     * malformed or too long hash).
     */
    template <bool HasPrefix>
    constexpr SemVer<HasPrefix> SemVer<HasPrefix>::from_string(
        std::string_view str
    )
    {
        const bool prefixed = detail::consume_char(str, 'v');
        if (prefixed != HasPrefix)
        {
            throw InvalidArgument(
                HasPrefix ? "Invalid version string: expected prefix 'v'"
                          : "Invalid version string: unexpected prefix 'v'"
            );
        }

        SemVerBase base{};
        if (!detail::consume_number(str, base.major) ||
            !detail::consume_char(str, '.') ||
            !detail::consume_number(str, base.minor) ||
            !detail::consume_char(str, '.') ||
            !detail::consume_number(str, base.patch))
        {
            throw InvalidArgument(
                "Invalid version string: expected 'major.minor.patch'"
            );
        }

        if (str.empty())
            return SemVer(base);

        if (!detail::consume_char(str, '+'))
        {
            throw InvalidArgument(
                "Invalid version string: unexpected trailing characters"
            );
        }

        // "1.2.3+" is malformed, whereas the constructor treats an empty hash
        // as "no hash"; the constructor checks alphabet and length.
        if (str.empty())
            throw InvalidArgument(
                "Invalid version string: empty hash extension"
            );

        return SemVer(base, str);
    }

    /**
     * @brief Converts the version to "[v]major.minor.patch[+hash]".
     */
    template <bool HasPrefix>
    inline std::string SemVer<HasPrefix>::to_string() const
    {
        std::string result;
        if constexpr (HasPrefix)
            result += 'v';

        result += std::to_string(_base.major) + "." +
                  std::to_string(_base.minor) + "." +
                  std::to_string(_base.patch);

        if (has_hash())
            result += "+" + std::string(hash());

        return result;
    }

    /**
     * @brief Partial ordering; see the class documentation. The hash never
     * orders versions, it can only make them `unordered`.
     */
    template <bool HasPrefix>
    constexpr std::partial_ordering SemVer<HasPrefix>::operator<=>(
        const SemVer& other
    ) const noexcept
    {
        if (const auto cmp = _base <=> other._base; cmp != 0)
            return cmp;

        return hash() == other.hash() ? std::partial_ordering::equivalent
                                      : std::partial_ordering::unordered;
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__SEMVER__SEMVER_TPP__
