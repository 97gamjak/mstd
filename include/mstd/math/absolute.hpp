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

#ifndef __MSTD__MATH__ABSOLUTE_HPP__
#define __MSTD__MATH__ABSOLUTE_HPP__

#include <cmath>
#include <cstdlib>
#include <type_traits>

#include "adl.hpp"

namespace mstd::detail
{
    using std::abs;
    using std::fabs;

    // fallbacks for arithmetic types
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto abs_impl(const T &x, adl_tag)
    {
        return abs(x);
    }
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto fabs_impl(const T &x, adl_tag)
    {
        return fabs(x);
    }
}   // namespace mstd::detail

namespace mstd
{
    // unqualified calls: ADL (through adl_tag) finds the hooks, including
    // the quantity overloads units.hpp adds later
    template <typename T>
    constexpr auto abs(const T &x)
    {
        return abs_impl(x, detail::adl_tag{});
    }
    template <typename T>
    constexpr auto fabs(const T &x)
    {
        return fabs_impl(x, detail::adl_tag{});
    }
}   // namespace mstd

#endif   // __MSTD__MATH__ABSOLUTE_HPP__
