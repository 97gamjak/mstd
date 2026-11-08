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

    /// quantity that is implicitly convertible to a plain number
    template <typename Q>
    concept dimensionless_quantity = requires(const Q &q) {
        q.numerical_value_in(::mp_units::one);
    };

    // exp/log are only defined for dimensionless arguments -> plain number
    template <auto R, typename Rep>
    requires dimensionless_quantity<::mp_units::quantity<R, Rep>>
    constexpr auto exp_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        return std::exp(q.numerical_value_in(::mp_units::one));
    }
    template <auto R, typename Rep>
    requires dimensionless_quantity<::mp_units::quantity<R, Rep>>
    constexpr auto log_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        return std::log(q.numerical_value_in(::mp_units::one));
    }

    // abs/fabs keep the quantity type, except dimensionless ones which
    // collapse to a plain number like the other math functions
#define MSTD_UNITS_ABS_HOOK(fn)                                              \
    template <auto R, typename Rep>                                          \
    constexpr auto fn##_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag) \
    {                                                                        \
        if constexpr (dimensionless_quantity<::mp_units::quantity<R, Rep>>)  \
            return std::fn(q.numerical_value_in(::mp_units::one));           \
        else                                                                 \
            return ::mp_units::abs(q);                                       \
    }

    MSTD_UNITS_ABS_HOOK(abs)
    MSTD_UNITS_ABS_HOOK(fabs)
#undef MSTD_UNITS_ABS_HOOK

    /// arithmetic type or dimensionless quantity
    template <typename T>
    concept number_like = std::is_arithmetic_v<T> || dimensionless_quantity<T>;

    template <auto R, typename Rep>
    inline constexpr bool
        is_dimensionless_quantity_v<::mp_units::quantity<R, Rep>> =
            dimensionless_quantity<::mp_units::quantity<R, Rep>>;

    template <typename T>
    constexpr auto to_number(const T &x)
    {
        if constexpr (std::is_arithmetic_v<T>)
            return x;
        else
            return x.numerical_value_in(::mp_units::one);
    }

    // sqrt/cbrt/pow: a dimensioned quantity stays a quantity (the dimension
    // changes). A dimensionless one collapses to a plain number: mp_units
    // would otherwise return e.g. quantity<sqrt(compressibility * pressure)>,
    // which is a distinct quantity type that does not convert to double.
    template <auto R, typename Rep>
    constexpr auto sqrt_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        if constexpr (dimensionless_quantity<::mp_units::quantity<R, Rep>>)
            return std::sqrt(q.numerical_value_in(::mp_units::one));
        else
            return ::mp_units::sqrt(q);
    }
    template <auto R, typename Rep>
    constexpr auto cbrt_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        if constexpr (dimensionless_quantity<::mp_units::quantity<R, Rep>>)
            return std::cbrt(q.numerical_value_in(::mp_units::one));
        else
            return ::mp_units::cbrt(q);
    }
    template <std::intmax_t Num, std::intmax_t Den = 1, auto R, typename Rep>
    constexpr auto pow_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag)
    {
        if constexpr (dimensionless_quantity<::mp_units::quantity<R, Rep>>)
            return pow_impl<Num, Den>(
                q.numerical_value_in(::mp_units::one),
                adl_tag{}
            );
        else
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
        detail::dimensionless_quantity<::mp_units::quantity<R, Rep>>
    )
    constexpr auto pow(const ::mp_units::quantity<R, Rep> &base, const U &exponent)
    {
        return std::pow(base.numerical_value_in(::mp_units::one), exponent);
    }

    /// max/min involving a dimensionless quantity: both arguments collapse to
    /// plain numbers (a double mixed with e.g. compressibility * pressure has
    /// no usable common type otherwise)
    template <typename A, typename B>
    requires(
        detail::number_like<A> && detail::number_like<B> &&
        (detail::is_dimensionless_quantity_v<A> ||
         detail::is_dimensionless_quantity_v<B>)
    )
    constexpr auto max(const A &a, const B &b)
    {
        return mstd::max(detail::to_number(a), detail::to_number(b));
    }
    template <typename A, typename B>
    requires(
        detail::number_like<A> && detail::number_like<B> &&
        (detail::is_dimensionless_quantity_v<A> ||
         detail::is_dimensionless_quantity_v<B>)
    )
    constexpr auto min(const A &a, const B &b)
    {
        return mstd::min(detail::to_number(a), detail::to_number(b));
    }
}   // namespace mstd

#endif   // __MSTD__UNITS_HPP__
