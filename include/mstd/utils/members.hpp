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

#ifndef __MSTD__UTILS__MEMBERS_HPP__
#define __MSTD__UTILS__MEMBERS_HPP__

#include "mstd/shared/traits/getter.hpp"   // IWYU pragma: keep -- for mstd::detail::getter_value

#define __CONST_GETTER__(name)                                   \
    [[nodiscard]] constexpr decltype(auto) name() const noexcept \
    {                                                            \
        return mstd::detail::getter_value(_##name);              \
    }

#define __CONST_PROPERTY_GETTER__(name)         \
    [[nodiscard]] decltype(auto) name()         \
        const noexcept(noexcept(_##name.get())) \
    {                                           \
        return _##name.get();                   \
    }

#define __SETTER__(name)                                                   \
    constexpr void                                                         \
    name(getter_t<std::remove_cvref_t<decltype(_##name)>> value) noexcept( \
        std::is_nothrow_copy_assignable_v<                                 \
            std::remove_cvref_t<decltype(_##name)>>                        \
    )                                                                      \
    {                                                                      \
        _##name = value;                                                   \
    }

#define __PROPERTY_SETTER__(name)                                             \
    void name(                                                                \
        getter_t<typename std::remove_cvref_t<decltype(_##name)>::value_type> \
            value                                                             \
    )                                                                         \
    {                                                                         \
        _##name.set(value);                                                   \
    }

#endif   // __MSTD__UTILS__MEMBERS_HPP__
