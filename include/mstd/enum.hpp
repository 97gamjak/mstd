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

#ifndef __MSTD__ENUM_HPP__
#define __MSTD__ENUM_HPP__

#include <array>         // IWYU pragma: keep
#include <cstddef>       // IWYU pragma: keep
#include <optional>      // IWYU pragma: keep
#include <span>          // IWYU pragma: keep
#include <string>        // IWYU pragma: keep
#include <string_view>   // IWYU pragma: keep
#include <type_traits>   // IWYU pragma: keep

#include "mstd/type_traits/enum_traits.hpp"   // IWYU pragma: keep

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
#define MSTD_ENUM(EnumName, Underlying, LIST)                               \
    enum class EnumName : Underlying                                        \
    {                                                                       \
        LIST(MSTD_ENUM_MAKE_ENUM)                                           \
    };                                                                      \
                                                                            \
    struct EnumName##Meta                                                   \
    {                                                                       \
        using type            = EnumName;                                   \
        using underlying_type = Underlying;                                 \
        using enum EnumName;                                                \
        static constexpr std::string_view EnumNameStr = #EnumName;          \
                                                                            \
        static constexpr auto values =                                      \
            std::to_array<EnumName>({LIST(MSTD_ENUM_MAKE_VALUE)});          \
                                                                            \
        static constexpr std::span<const EnumName> values_view()            \
        {                                                                   \
            return values;                                                  \
        }                                                                   \
                                                                            \
        static constexpr auto names =                                       \
            std::to_array<std::string_view>({LIST(MSTD_ENUM_MAKE_STRING)}); \
                                                                            \
        static constexpr std::size_t size = values.size();                  \
                                                                            \
        static constexpr auto begin() { return values.begin(); }            \
        static constexpr auto end() { return values.end(); }                \
                                                                            \
        static constexpr std::string_view name(EnumName enum)               \
        {                                                                   \
            for (std::size_t i = 0; i < size; ++i)                          \
                if (values.at(i) == enum)                                   \
                    return names.at(i);                                     \
            return {};                                                      \
        }                                                                   \
                                                                            \
        static constexpr std::string toString(EnumName enum)                \
        {                                                                   \
            for (std::size_t i = 0; i < size; ++i)                          \
                if (values.at(i) == enum)                                   \
                    return std::string(names.at(i));                        \
            return {};                                                      \
        }                                                                   \
                                                                            \
        static constexpr std::optional<EnumName> from_string(               \
            std::string_view str                                            \
        )                                                                   \
        {                                                                   \
            for (std::size_t i = 0; i < size; ++i)                          \
                if (names.at(i) == str)                                     \
                    return values.at(i);                                    \
            return std::nullopt;                                            \
        }                                                                   \
                                                                            \
        static constexpr underlying_type to_underlying(EnumName enum)       \
        {                                                                   \
            return static_cast<underlying_type>(enum);                      \
        }                                                                   \
                                                                            \
        static constexpr std::optional<std::size_t> index(EnumName enum)    \
        {                                                                   \
            for (std::size_t i = 0; i < size; ++i)                          \
                if (values.at(i) == enum)                                   \
                    return i;                                               \
            return std::nullopt;                                            \
        }                                                                   \
    };                                                                      \
                                                                            \
    static constexpr EnumName##Meta enum_meta(EnumName) { return {}; }
#else
#define MSTD_ENUM(EnumName, Underlying, LIST) enum class EnumName : Underlying;
#endif

#define MSTD_ENUM_BITFLAG(EnumName, Underlying, LIST)                         \
    MSTD_ENUM(EnumName, Underlying, LIST)                                     \
                                                                              \
    inline constexpr EnumName operator|(EnumName lhs, EnumName rhs)           \
    {                                                                         \
        return static_cast<EnumName>(                                         \
            static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs)       \
        );                                                                    \
    }                                                                         \
    inline constexpr EnumName& operator|=(EnumName& lhs, EnumName rhs)        \
    {                                                                         \
        lhs = lhs | rhs;                                                      \
        return lhs;                                                           \
    }                                                                         \
                                                                              \
    struct EnumName##FlagTest                                                 \
    {                                                                         \
        Underlying value;                                                     \
        constexpr  operator EnumName() const noexcept                         \
        {                                                                     \
            return static_cast<EnumName>(value);                              \
        }                                                                     \
                                                                              \
        constexpr explicit operator bool() const noexcept                     \
        {                                                                     \
            return value != Underlying{0};                                    \
        }                                                                     \
    };                                                                        \
                                                                              \
    inline constexpr EnumName##FlagTest operator&(EnumName lhs, EnumName rhs) \
    {                                                                         \
        return EnumName##FlagTest{static_cast<Underlying>(                    \
            static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs)       \
        )};                                                                   \
    }                                                                         \
                                                                              \
    inline constexpr bool operator!(EnumName lhs)                             \
    {                                                                         \
        return !static_cast<Underlying>(lhs);                                 \
    }

#endif   // __MSTD__ENUM_HPP__
