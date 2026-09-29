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

#ifndef __MSTD__ENUM__ENUM_HPP__
#define __MSTD__ENUM__ENUM_HPP__

#include <array>         // IWYU pragma: keep
#include <cstddef>       // IWYU pragma: keep
#include <optional>      // IWYU pragma: keep
#include <span>          // IWYU pragma: keep
#include <string>        // IWYU pragma: keep
#include <string_view>   // IWYU pragma: keep
#include <type_traits>   // IWYU pragma: keep

#include "mstd/enum/enum_string.hpp"          // IWYU pragma: keep
#include "mstd/type_traits/enum_traits.hpp"   // IWYU pragma: keep
#include "mstd/type_traits/string.hpp"        // IWYU pragma: keep

//
// Element expanders
//
// X(Name)       -> Name,
// X(Name, 42)   -> Name = 42,
#define MSTD_ENUM_MAKE_ENUM(name, ...) name __VA_OPT__(= __VA_ARGS__),

// X(Name) or X(Name, 42) -> EnumName::Name,
#define MSTD_ENUM_MAKE_VALUE(name, ...) name,

// X(Name) -> "Name",
#define MSTD_ENUM_MAKE_STRING(name, ...) #name,

// ------------------------------------------------------------
// Main macro
// ------------------------------------------------------------
#ifndef Q_MOC_RUN
#define MSTD_ENUM(EnumName, Underlying, LIST)                                  \
    enum class EnumName : Underlying                                           \
    {                                                                          \
        LIST(MSTD_ENUM_MAKE_ENUM)                                              \
    };                                                                         \
                                                                               \
    struct EnumName##Meta                                                      \
    {                                                                          \
        using type            = EnumName;                                      \
        using underlying_type = Underlying;                                    \
        using enum EnumName;                                                   \
        static constexpr std::string_view EnumNameStr = #EnumName;             \
                                                                               \
        static constexpr auto values =                                         \
            std::to_array<EnumName>({LIST(MSTD_ENUM_MAKE_VALUE)});             \
                                                                               \
        static constexpr std::span<const EnumName> values_view()               \
        {                                                                      \
            return values;                                                     \
        }                                                                      \
                                                                               \
        static constexpr auto originalNames =                                  \
            std::to_array<std::string_view>({LIST(MSTD_ENUM_MAKE_STRING)});    \
                                                                               \
        static constexpr std::size_t size = values.size();                     \
                                                                               \
        /* effective spellings: originalNames with EnumNames overrides    */   \
        /* applied -- a function, not a plain array, since EnumNames<E>   */   \
        /* may be specialized after this macro                           */    \
        template <typename Self = EnumName##Meta>                              \
        static constexpr std::array<std::string_view, size> names()            \
        {                                                                      \
            static_assert(                                                     \
                mstd::detail::hasCheckedTables<Self>(),                        \
                "EnumAliases<E>::value/EnumNames<E>::value must be "           \
                "constructed via mstd::makeAliases/mstd::makeNames, not a "    \
                "hand-written std::array, so clashes are actually checked"     \
            );                                                                 \
            static_assert(                                                     \
                !mstd::detail::hasAliasClash<Self>(),                          \
                "alias clashes with another alias, an enumerator name, or "    \
                "an EnumNames override (compared case-insensitively)"          \
            );                                                                 \
                                                                               \
            std::array<std::string_view, size> out = originalNames;            \
            for (const auto& [value, text] :                                   \
                 mstd::EnumNames<typename Self::type>::value)                  \
                for (std::size_t i = 0; i < size; ++i)                         \
                    if (values.at(i) == value)                                 \
                        out.at(i) = text;                                      \
            return out;                                                        \
        }                                                                      \
                                                                               \
        static constexpr auto begin() { return values.begin(); }               \
        static constexpr auto end() { return values.end(); }                   \
                                                                               \
        template <typename Self = EnumName##Meta>                              \
        static constexpr std::string_view name(EnumName enum_)                 \
        {                                                                      \
            static_assert(                                                     \
                mstd::detail::hasCheckedTables<Self>(),                        \
                "EnumAliases<E>::value/EnumNames<E>::value must be "           \
                "constructed via mstd::makeAliases/mstd::makeNames, not a "    \
                "hand-written std::array, so clashes are actually checked"     \
            );                                                                 \
            static_assert(                                                     \
                !mstd::detail::hasAliasClash<Self>(),                          \
                "alias clashes with another alias, an enumerator name, or "    \
                "an EnumNames override (compared case-insensitively)"          \
            );                                                                 \
                                                                               \
            for (const auto& [value, text] :                                   \
                 mstd::EnumNames<typename Self::type>::value)                  \
                if (value == enum_)                                            \
                    return text;                                               \
                                                                               \
            for (std::size_t i = 0; i < size; ++i)                             \
                if (values.at(i) == enum_)                                     \
                    return originalNames.at(i);                                \
            return {};                                                         \
        }                                                                      \
                                                                               \
        template <typename Self = EnumName##Meta>                              \
        static constexpr std::string toString(EnumName enum_)                  \
        {                                                                      \
            return std::string(Self::template name<Self>(enum_));              \
        }                                                                      \
                                                                               \
        /* member templates: EnumNames/EnumAliases are looked up at call    */ \
        /* time, so they may be specialized after this macro                */ \
        template <typename Self = EnumName##Meta>                              \
        static constexpr std::optional<EnumName> from_string(                  \
            std::string_view str                                               \
        )                                                                      \
        {                                                                      \
            static_assert(                                                     \
                mstd::detail::hasCheckedTables<Self>(),                        \
                "EnumAliases<E>::value/EnumNames<E>::value must be "           \
                "constructed via mstd::makeAliases/mstd::makeNames, not a "    \
                "hand-written std::array, so clashes are actually checked"     \
            );                                                                 \
            static_assert(                                                     \
                !mstd::detail::hasAliasClash<Self>(),                          \
                "alias clashes with another alias, an enumerator name, or "    \
                "an EnumNames override (compared case-insensitively)"          \
            );                                                                 \
                                                                               \
            for (std::size_t i = 0; i < size; ++i)                             \
                if (originalNames.at(i) == str)                                \
                    return values.at(i);                                       \
            for (const auto& [value, text] :                                   \
                 mstd::EnumNames<typename Self::type>::value)                  \
                if (text == str)                                               \
                    return value;                                              \
            for (const auto& [alias, value] :                                  \
                 mstd::EnumAliases<typename Self::type>::value)                \
                if (alias == str)                                              \
                    return value;                                              \
            return std::nullopt;                                               \
        }                                                                      \
                                                                               \
        template <typename Self = EnumName##Meta>                              \
        static constexpr std::optional<EnumName> from_stringCaseInsensitive(   \
            std::string_view str                                               \
        )                                                                      \
        {                                                                      \
            static_assert(                                                     \
                !mstd::hasCaseInsensitiveCollision<(size)>(originalNames),     \
                "from_stringCaseInsensitive is ambiguous: this enum has "      \
                "enumerator names that differ only by case"                    \
            );                                                                 \
            static_assert(                                                     \
                mstd::detail::hasCheckedTables<Self>(),                        \
                "EnumAliases<E>::value/EnumNames<E>::value must be "           \
                "constructed via mstd::makeAliases/mstd::makeNames, not a "    \
                "hand-written std::array, so clashes are actually checked"     \
            );                                                                 \
            static_assert(                                                     \
                !mstd::detail::hasAliasClash<Self>(),                          \
                "alias clashes with another alias, an enumerator name, or "    \
                "an EnumNames override (compared case-insensitively)"          \
            );                                                                 \
                                                                               \
            for (std::size_t i = 0; i < originalNames.size(); ++i)             \
                if (mstd::iequals(originalNames.at(i), str))                   \
                    return values.at(i);                                       \
            for (const auto& [value, text] :                                   \
                 mstd::EnumNames<typename Self::type>::value)                  \
                if (mstd::iequals(text, str))                                  \
                    return value;                                              \
            for (const auto& [alias, value] :                                  \
                 mstd::EnumAliases<typename Self::type>::value)                \
                if (mstd::iequals(alias, str))                                 \
                    return value;                                              \
            return std::nullopt;                                               \
        }                                                                      \
                                                                               \
        static constexpr underlying_type to_underlying(EnumName enum_)         \
        {                                                                      \
            return static_cast<underlying_type>(enum_);                        \
        }                                                                      \
                                                                               \
        static constexpr std::optional<std::size_t> index(EnumName enum_)      \
        {                                                                      \
            for (std::size_t i = 0; i < size; ++i)                             \
                if (values.at(i) == enum_)                                     \
                    return i;                                                  \
            return std::nullopt;                                               \
        }                                                                      \
    };                                                                         \
                                                                               \
    inline constexpr EnumName##Meta enum_meta(EnumName) { return {}; }

#else
#define MSTD_ENUM(EnumName, Underlying, LIST) enum class EnumName : Underlying;
#endif

#endif   // __MSTD__ENUM__ENUM_HPP__
