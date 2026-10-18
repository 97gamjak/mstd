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

#define MSTD_DETAIL_CONST_GETTER(fn, name)                     \
    [[nodiscard]] constexpr decltype(auto) fn() const noexcept \
    {                                                          \
        return ::mstd::detail::getter_value(_##name);          \
    }

#define MSTD_DETAIL_CONST_PROPERTY_GETTER(fn, name)                           \
    [[nodiscard]] decltype(auto) fn() const noexcept(noexcept(_##name.get())) \
    {                                                                         \
        return _##name.get();                                                 \
    }

#define MSTD_DETAIL_SETTER(fn, name)                                   \
    constexpr void fn(                                                 \
        ::mstd::getter_t<std::remove_cvref_t<decltype(_##name)>> value \
    ) noexcept(std::                                                   \
                   is_nothrow_copy_assignable_v<                       \
                       std::remove_cvref_t<decltype(_##name)>>)        \
    {                                                                  \
        _##name = value;                                               \
    }

#define MSTD_DETAIL_PROPERTY_SETTER(fn, name)                                  \
    void fn(                                                                   \
        ::mstd::getter_t<                                                      \
            typename std::remove_cvref_t<decltype(_##name)>::value_type> value \
    )                                                                          \
    {                                                                          \
        _##name.set(value);                                                    \
    }

// ---- public scope: name() / name(value) ----

#define CONST_GETTER(name) MSTD_DETAIL_CONST_GETTER(name, name)
#define CONST_PROPERTY_GETTER(name) \
    MSTD_DETAIL_CONST_PROPERTY_GETTER(name, name)
#define SETTER(name)          MSTD_DETAIL_SETTER(name, name)
#define PROPERTY_SETTER(name) MSTD_DETAIL_PROPERTY_SETTER(name, name)

// ---- protected / private scope: _name_() / _name_(value) ----

#define INTERNAL_CONST_GETTER(name) MSTD_DETAIL_CONST_GETTER(_##name##_, name)
#define INTERNAL_CONST_PROPERTY_GETTER(name) \
    MSTD_DETAIL_CONST_PROPERTY_GETTER(_##name##_, name)
#define INTERNAL_SETTER(name) MSTD_DETAIL_SETTER(_##name##_, name)
#define INTERNAL_PROPERTY_SETTER(name) \
    MSTD_DETAIL_PROPERTY_SETTER(_##name##_, name)

#endif   // __MSTD__UTILS__MEMBERS_HPP__
