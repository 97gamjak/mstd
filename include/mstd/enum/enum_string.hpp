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

#ifndef __MSTD__ENUM__ENUM_STRING_HPP__
#define __MSTD__ENUM__ENUM_STRING_HPP__

#include <array>
#include <cstddef>
#include <string_view>
#include <utility>

#include "mstd/type_traits/string.hpp"

namespace mstd
{
    /**
     * Optional per-enum customization points for MSTD_ENUM enums.
     * Specialize in the SAME header, directly after the MSTD_ENUM(...) line.
     *
     *   template <> struct mstd::EnumAliases<E> {
     *       static constexpr std::array<std::pair<std::string_view, E>, N>
     * value{{...}};
     *   };
     */
    template <typename E>
    struct EnumAliases
    {
        static constexpr std::array<std::pair<std::string_view, E>, 0> value{};
    };

    /**
     * Overrides the generated spelling for individual enumerators (toString
     * / name()). Entries not covered here keep their generated name.
     * The generated name always still parses via from_string, in addition
     * to whatever is registered here.
     */
    template <typename E>
    struct EnumNames
    {
        static constexpr std::array<std::pair<E, std::string_view>, 0> value{};
    };

    namespace detail
    {
        // alias equals (case-insensitively) another alias or an enumerator name
        template <typename Meta>
        constexpr bool hasAliasClash()
        {
            constexpr auto& al = EnumAliases<typename Meta::type>::value;
            for (std::size_t i = 0; i < al.size(); ++i)
            {
                for (std::size_t j = 0; j < i; ++j)
                    if (iequals(al[i].first, al[j].first))
                        return true;
                for (const auto& name : Meta::originalNames)
                    if (iequals(al[i].first, name))
                        return true;
            }
            return false;
        }

    }   // namespace detail

    /**
     * Checked constructors: evaluated where the specialization is written,
     * so a clash is a compile error even if nothing ever calls from_string.
     *
     *   static constexpr auto value = mstd::makeAliases<E>({{"MOL", E::X},
     * ...});
     */
    template <typename E, std::size_t N>
    consteval std::array<std::pair<std::string_view, E>, N> makeAliases(
        const std::pair<std::string_view, E> (&list)[N]
    )
    {
        using Meta = decltype(enum_meta(E{}));

        std::array<std::pair<std::string_view, E>, N> out{};
        for (std::size_t i = 0; i < N; ++i)
        {
            out[i] = list[i];
            for (std::size_t j = 0; j < i; ++j)
                if (iequals(list[i].first, list[j].first))
                    throw "duplicate alias (case-insensitive)";
            for (const auto& name : Meta::originalNames)
                if (iequals(list[i].first, name))
                    throw "alias equals an enumerator name (case-insensitive)";
        }
        return out;
    }

    /**
     * Checked constructor for EnumNames: evaluated where the specialization
     * is written, so a clash is a compile error even if nothing ever calls
     * from_string / toString.
     */
    template <typename E, std::size_t N>
    consteval std::array<std::pair<E, std::string_view>, N> makeNames(
        const std::pair<E, std::string_view> (&list)[N]
    )
    {
        using Meta = decltype(enum_meta(E{}));

        std::array<std::pair<E, std::string_view>, N> out{};
        for (std::size_t i = 0; i < N; ++i)
        {
            out[i] = list[i];
            for (std::size_t j = 0; j < i; ++j)
                if (iequals(list[i].second, list[j].second))
                    throw "duplicate overridden name (case-insensitive)";
            for (std::size_t k = 0; k < Meta::size; ++k)
                if (Meta::values.at(k) != list[i].first &&
                    iequals(list[i].second, Meta::originalNames.at(k)))
                    throw "overridden name equals another enumerator's "
                          "original name (case-insensitive)";
        }
        return out;
    }
}   // namespace mstd

#endif   // __MSTD__ENUM__ENUM_STRING_HPP__
