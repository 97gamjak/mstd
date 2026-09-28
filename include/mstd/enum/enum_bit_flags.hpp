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

#ifndef __MSTD__ENUM__ENUM_BIT_FLAGS_HPP__
#define __MSTD__ENUM__ENUM_BIT_FLAGS_HPP__

#include "enum.hpp"

#define MSTD_ENUM_BITFLAG(EnumName, Underlying, LIST)                         \
    MSTD_ENUM(EnumName, Underlying, LIST)                                     \
                                                                              \
    inline constexpr EnumName operator|(EnumName lhs, EnumName rhs)           \
    {                                                                         \
        return static_cast<EnumName>(                                         \
            static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs)       \
        );                                                                    \
    }                                                                         \
                                                                              \
    inline constexpr EnumName& operator|=(EnumName& lhs, EnumName rhs)        \
    {                                                                         \
        lhs = lhs | rhs;                                                      \
        return lhs;                                                           \
    }                                                                         \
                                                                              \
    inline constexpr EnumName operator~(EnumName value)                       \
    {                                                                         \
        return static_cast<EnumName>(~static_cast<Underlying>(value));        \
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
    inline constexpr EnumName& operator&=(EnumName& lhs, EnumName rhs)        \
    {                                                                         \
        lhs = lhs & rhs;                                                      \
        return lhs;                                                           \
    }                                                                         \
                                                                              \
    inline constexpr bool operator!(EnumName lhs)                             \
    {                                                                         \
        return !static_cast<Underlying>(lhs);                                 \
    }

#endif   // __MSTD__ENUM__ENUM_BIT_FLAGS_HPP__
