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

#ifndef __MSTD__MATH__MINMAX_HPP__
#define __MSTD__MATH__MINMAX_HPP__

#include <type_traits>

namespace mstd::detail
{
    template <typename A, typename B>
    concept has_common_type = requires { typename std::common_type_t<A, B>; };

    /// customization point: units.hpp specializes this to true for
    /// dimensionless quantities, which max/min collapse to plain numbers
    template <typename T>
    inline constexpr bool is_dimensionless_quantity_v = false;

    template <typename A, typename B>
    concept plain_minmax_args =
        has_common_type<A, B> && !is_dimensionless_quantity_v<A> &&
        !is_dimensionless_quantity_v<B>;
}   // namespace mstd::detail

namespace mstd
{
    /// larger of a and b, returned by value as their common type (so it also
    /// works for mixed arithmetic types and quantities with different units)
    template <typename A, typename B>
    requires detail::plain_minmax_args<A, B>
    constexpr auto max(const A &a, const B &b)
    {
        using C = std::common_type_t<A, B>;
        return (C(a) < C(b)) ? C(b) : C(a);
    }

    /// smaller of a and b, see max
    template <typename A, typename B>
    requires detail::plain_minmax_args<A, B>
    constexpr auto min(const A &a, const B &b)
    {
        using C = std::common_type_t<A, B>;
        return (C(b) < C(a)) ? C(b) : C(a);
    }
}   // namespace mstd

#endif   // __MSTD__MATH__MINMAX_HPP__
