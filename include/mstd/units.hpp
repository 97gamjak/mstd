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

#ifndef __MSTD__UNITS_HPP__
#define __MSTD__UNITS_HPP__

#include <cmath>
#include <cstdint>
#include <type_traits>
#include <mp-units/math.h>
#include <mp-units/systems/si/math.h>

#include "mstd/math.hpp"               // needed for mstd::detail::adl_tag
#include "units/namespace.hpp"         // IWYU pragma: export
#include "units/new_units.hpp"         // IWYU pragma: export
#include "units/relaxation_time.hpp"   // IWYU pragma: export
#include "units/wave_number.hpp"       // IWYU pragma: export

namespace mstd::detail
{
    // quantity overloads: more specialized than the generic fallback, so they
    // win
#define MSTD_UNITS_TRIG_HOOK(fn)                                             \
    template <auto R, typename Rep>                                          \
    constexpr auto fn##_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag) \
    {                                                                        \
        return ::mp_units::si::fn(q).numerical_value_in(::mp_units::one);    \
    }

    MSTD_UNITS_TRIG_HOOK(sin)
    MSTD_UNITS_TRIG_HOOK(cos)
    MSTD_UNITS_TRIG_HOOK(tan)
#undef MSTD_UNITS_TRIG_HOOK

    // dimension-changing functions: the result stays a quantity
    template <auto R, typename Rep>
    constexpr auto sqrt_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        return ::mp_units::sqrt(q);
    }
    template <auto R, typename Rep>
    constexpr auto cbrt_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        return ::mp_units::cbrt(q);
    }
    template <std::intmax_t Num, std::intmax_t Den = 1, auto R, typename Rep>
    constexpr auto pow_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        return ::mp_units::pow<Num, Den>(q);
    }
}   // namespace mstd::detail

namespace mstd
{
    /// runtime exponent on a dimensionless quantity (e.g. compressibility *
    /// pressure): the result is a plain number, like the trig functions
    template <auto R, typename Rep, typename U>
    requires(
        std::is_arithmetic_v<U> &&
        requires(const ::mp_units::quantity<R, Rep> &q) {
            q.numerical_value_in(::mp_units::one);
        }
    )
    constexpr auto pow(const ::mp_units::quantity<R, Rep> &base, const U &exponent)
    {
        return std::pow(base.numerical_value_in(::mp_units::one), exponent);
    }
}   // namespace mstd

#endif   // __MSTD__UNITS_HPP__
