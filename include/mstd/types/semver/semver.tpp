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

#include <charconv>
#include <utility>

#include "mstd/exceptions.hpp"
#include "semver.hpp"

namespace mstd::detail
{
    /**
     * @brief Parses an unsigned decimal number from the front of `str` and
     * removes it from `str`. Rejects empty input, signs and overflow.
     */
    inline bool consume_number(std::string_view& str, size_t& out)
    {
        const char* first = str.data();
        const char* last  = first + str.size();

        const auto [ptr, ec] = std::from_chars(first, last, out);
        if (ec != std::errc{} || ptr == first)
            return false;

        str.remove_prefix(static_cast<size_t>(ptr - first));
        return true;
    }

    inline bool consume_char(std::string_view& str, char c)
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
    inline bool is_valid_hash_extension(std::string_view str)
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
     * @brief Constructs a version from its components.
     *
     * @param base          Major, minor and patch numbers.
     * @param hashExtension Optional hash / build metadata (without the '+').
     */
    template <bool HasPrefix>
    inline SemVer<HasPrefix>::SemVer(
        SemVerBase                 base,
        std::optional<std::string> hashExtension
    )
        : _base(base), _hashExtension(std::move(hashExtension))
    {
    }

    template <bool HasPrefix>
    inline SemVer<HasPrefix> SemVer<HasPrefix>::from_string(
        std::string_view str
    )
    {
        const std::string_view original = str;
        const auto             fail     = [&original](const char* reason)
        {
            return InvalidArgument(
                std::string("Invalid version string '") +
                std::string(original) + "': " + reason
            );
        };

        const bool prefixed = detail::consume_char(str, 'v');
        if (prefixed != HasPrefix)
        {
            throw fail(
                HasPrefix ? "expected prefix 'v'" : "unexpected prefix 'v'"
            );
        }

        SemVerBase base{};
        if (!detail::consume_number(str, base.major) ||
            !detail::consume_char(str, '.') ||
            !detail::consume_number(str, base.minor) ||
            !detail::consume_char(str, '.') ||
            !detail::consume_number(str, base.patch))
        {
            throw fail("expected 'major.minor.patch'");
        }

        if (str.empty())
            return SemVer(base);

        if (!detail::consume_char(str, '+'))
            throw fail("unexpected trailing characters");

        if (!detail::is_valid_hash_extension(str))
            throw fail("malformed hash extension");

        return SemVer(base, std::string(str));
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

        if (_hashExtension.has_value())
            result += "+" + *_hashExtension;

        return result;
    }

    /**
     * @brief Partial ordering; see the class documentation. The hash never
     * orders versions, it can only make them `unordered`.
     */
    template <bool HasPrefix>
    inline std::partial_ordering SemVer<HasPrefix>::operator<=>(
        const SemVer& other
    ) const
    {
        if (const auto cmp = _base <=> other._base; cmp != 0)
            return cmp;

        return _hashExtension == other._hashExtension
                   ? std::partial_ordering::equivalent
                   : std::partial_ordering::unordered;
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__SEMVER__SEMVER_TPP__
