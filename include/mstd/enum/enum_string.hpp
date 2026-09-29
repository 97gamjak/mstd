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
#include <type_traits>
#include <utility>

#include "mstd/type_traits/string.hpp"

namespace mstd
{
    template <typename E>
    struct AliasEntry
    {
        std::string_view text;
        E                value;
    };

    template <typename E>
    struct NameEntry
    {
        E                value;
        std::string_view text;
    };

    /**
     * The checked container type for EnumAliases<E>::value. Only
     * mstd::makeAliases(...) constructs one of these -- there is no public
     * way to build one from an unchecked list, which is what makes the
     * is_alias_table_v check below meaningful: a specialization whose
     * `value` is any other type (e.g. a raw std::array written by hand)
     * never went through the clash checks in makeAliases at all.
     */
    template <typename E, std::size_t N>
    class AliasTable
    {
       public:
        constexpr AliasTable()
        requires(N == 0)
        = default;

        constexpr auto        begin() const { return _entries.begin(); }
        constexpr auto        end() const { return _entries.end(); }
        constexpr std::size_t size() const { return N; }
        constexpr const AliasEntry<E>& operator[](std::size_t i) const
        {
            return _entries[i];
        }

       private:
        template <typename E2, std::size_t N2>
        friend consteval AliasTable<E2, N2> makeAliases(
            const std::pair<std::string_view, E2> (&)[N2]
        );

        constexpr explicit AliasTable(std::array<AliasEntry<E>, N> entries)
            : _entries(entries)
        {
        }

        std::array<AliasEntry<E>, N> _entries{};
    };

    /**
     * The checked container type for EnumNames<E>::value -- see AliasTable;
     * only mstd::makeNames(...) can construct one.
     */
    template <typename E, std::size_t N>
    class NameTable
    {
       public:
        constexpr NameTable()
        requires(N == 0)
        = default;

        constexpr auto                begin() const { return _entries.begin(); }
        constexpr auto                end() const { return _entries.end(); }
        constexpr std::size_t         size() const { return N; }
        constexpr const NameEntry<E>& operator[](std::size_t i) const
        {
            return _entries[i];
        }

       private:
        template <typename E2, std::size_t N2>
        friend consteval NameTable<E2, N2> makeNames(
            const std::pair<E2, std::string_view> (&)[N2]
        );

        constexpr explicit NameTable(std::array<NameEntry<E>, N> entries)
            : _entries(entries)
        {
        }

        std::array<NameEntry<E>, N> _entries{};
    };

    namespace detail
    {
        template <typename T>
        struct is_alias_table : std::false_type
        {
        };

        template <typename E, std::size_t N>
        struct is_alias_table<AliasTable<E, N>> : std::true_type
        {
        };

        template <typename T>
        inline constexpr bool is_alias_table_v =
            is_alias_table<std::remove_cvref_t<T>>::value;

        template <typename T>
        struct is_name_table : std::false_type
        {
        };

        template <typename E, std::size_t N>
        struct is_name_table<NameTable<E, N>> : std::true_type
        {
        };

        template <typename T>
        inline constexpr bool is_name_table_v =
            is_name_table<std::remove_cvref_t<T>>::value;
    }   // namespace detail

    /**
     * Optional per-enum customization points for MSTD_ENUM enums.
     * Specialize in the SAME header, directly after the MSTD_ENUM(...) line,
     * using mstd::makeAliases(...) -- NOT a hand-written std::array -- so
     * the clash checks below actually ran:
     *
     *   template <> struct mstd::EnumAliases<E> {
     *       static constexpr auto value =
     *           mstd::makeAliases<E>({{"alias", E::X}, ...});
     *   };
     */
    template <typename E>
    struct EnumAliases
    {
        static constexpr AliasTable<E, 0> value{};
    };

    /**
     * Overrides the generated spelling for individual enumerators (toString
     * / name()). Entries not covered here keep their generated name.
     * The generated name always still parses via from_string, in addition
     * to whatever is registered here. Specialize using mstd::makeNames(...),
     * not a hand-written std::array -- see EnumAliases above.
     */
    template <typename E>
    struct EnumNames
    {
        static constexpr NameTable<E, 0> value{};
    };

    namespace detail
    {
        // both EnumAliases<E>::value and EnumNames<E>::value are of the
        // required checked type -- i.e. both were actually constructed via
        // mstd::makeAliases/mstd::makeNames, not a hand-written std::array
        // that bypasses their clash checks entirely
        template <typename Meta>
        constexpr bool hasCheckedTables()
        {
            return is_alias_table_v<
                       decltype(EnumAliases<typename Meta::type>::value)> &&
                   is_name_table_v<
                       decltype(EnumNames<typename Meta::type>::value)>;
        }

        // alias equals (case-insensitively) another alias, an enumerator's
        // original name, or an EnumNames override registered for a
        // DIFFERENT value.
        //
        // Guarded by hasCheckedTables: if EnumAliases<E>::value or
        // EnumNames<E>::value is not the checked table type (e.g. a
        // hand-written std::array bypassing makeAliases/makeNames), the
        // body below -- which assumes AliasEntry/NameEntry's .text/.value
        // members -- is never instantiated. Without this guard, a
        // mismatched type produces a pile of unrelated hard errors (no
        // member named 'text' in 'std::pair<...>') on top of, and
        // obscuring, the one actionable hasCheckedTables diagnostic.
        template <typename Meta>
        constexpr bool hasAliasClash()
        {
            if constexpr (!hasCheckedTables<Meta>())
            {
                return false;
            }
            else
            {
                constexpr auto& al = EnumAliases<typename Meta::type>::value;
                constexpr auto& ov = EnumNames<typename Meta::type>::value;
                for (std::size_t i = 0; i < al.size(); ++i)
                {
                    for (std::size_t j = 0; j < i; ++j)
                        if (iequals(al[i].text, al[j].text))
                            return true;
                    for (const auto& name : Meta::originalNames)
                        if (iequals(al[i].text, name))
                            return true;
                    for (const auto& entry : ov)
                        if (al[i].value != entry.value &&
                            iequals(al[i].text, entry.text))
                            return true;
                }
                return false;
            }
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
    consteval AliasTable<E, N> makeAliases(
        const std::pair<std::string_view, E> (&list)[N]
    )
    {
        using Meta = decltype(enum_meta(E{}));

        std::array<AliasEntry<E>, N> out{};
        for (std::size_t i = 0; i < N; ++i)
        {
            out[i] = AliasEntry<E>{list[i].first, list[i].second};
            for (std::size_t j = 0; j < i; ++j)
                if (iequals(list[i].first, list[j].first))
                    throw "duplicate alias (case-insensitive)";
            for (const auto& name : Meta::originalNames)
                if (iequals(list[i].first, name))
                    throw "alias equals an enumerator name (case-insensitive)";
        }
        return AliasTable<E, N>{out};
    }

    /**
     * Checked constructor for EnumNames: evaluated where the specialization
     * is written, so a clash is a compile error even if nothing ever calls
     * from_string / toString.
     */
    template <typename E, std::size_t N>
    consteval NameTable<E, N> makeNames(
        const std::pair<E, std::string_view> (&list)[N]
    )
    {
        using Meta = decltype(enum_meta(E{}));

        std::array<NameEntry<E>, N> out{};
        for (std::size_t i = 0; i < N; ++i)
        {
            out[i] = NameEntry<E>{list[i].first, list[i].second};
            for (std::size_t j = 0; j < i; ++j)
                if (iequals(list[i].second, list[j].second))
                    throw "duplicate overridden name (case-insensitive)";
            for (std::size_t k = 0; k < Meta::size; ++k)
                if (Meta::values.at(k) != list[i].first &&
                    iequals(list[i].second, Meta::originalNames.at(k)))
                    throw "overridden name equals another enumerator's "
                          "original name (case-insensitive)";
        }
        return NameTable<E, N>{out};
    }
}   // namespace mstd

#endif   // __MSTD__ENUM__ENUM_STRING_HPP__
