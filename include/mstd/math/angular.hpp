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

#ifndef __MSTD__MATH__ANGULAR_HPP__
#define __MSTD__MATH__ANGULAR_HPP__

#include <cmath>

namespace mstd::detail
{
    // Tag type: makes ADL search mstd::detail, where the *_impl hooks live
    struct adl_tag
    {
    };

    using std::cos;
    using std::sin;
    using std::tan;

    // fallback for arithmetic types
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto sin_impl(const T &x, adl_tag)
    {
        return sin(x);
    }
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto cos_impl(const T &x, adl_tag)
    {
        return cos(x);
    }
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto tan_impl(const T &x, adl_tag)
    {
        return tan(x);
    }
}   // namespace mstd::detail

namespace mstd
{
    // unqualified call: ADL (through adl_tag) finds the hooks, including
    // the ones units.hpp adds later
    template <typename T>
    constexpr auto sin(const T &x)
    {
        return sin_impl(x, detail::adl_tag{});
    }
    template <typename T>
    constexpr auto cos(const T &x)
    {
        return cos_impl(x, detail::adl_tag{});
    }
    template <typename T>
    constexpr auto tan(const T &x)
    {
        return tan_impl(x, detail::adl_tag{});
    }
}   // namespace mstd

#undef MSTD_MATH_UNARY

#endif   // __MSTD__MATH__ANGULAR_HPP__
