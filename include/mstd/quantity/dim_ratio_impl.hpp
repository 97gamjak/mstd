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

#ifndef __MSTD_DIM_RATIO_IMPL_HPP__
#define __MSTD_DIM_RATIO_IMPL_HPP__

#include "dim_ratio.hpp"
#include "dim_ratio_operations.hpp"

/**
 * @file dim_ratio_impl.hpp
 * @brief Implementations for common DimRatio types.
 *
 * Provides predefined DimRatio types for common physical dimensions
 * such as length, mass, time, current, temperature, amount, luminous intensity,
 * and angle, each with an optional scaling ratio.
 */

namespace mstd
{
    template <RatioType R1 = Rational<1, 1>>
    struct length_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_length, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using length_dim_ratio = typename length_dim_ratio_impl<R1>::type;

    template <RatioType R1 = Rational<1, 1>>
    struct mass_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_mass, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using mass_dim_ratio = typename mass_dim_ratio_impl<R1>::type;

    template <RatioType R1 = Rational<1, 1>>
    struct time_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_time, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using time_dim_ratio = typename time_dim_ratio_impl<R1>::type;

    template <RatioType R1 = Rational<1, 1>>
    struct current_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_current, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using current_dim_ratio = typename current_dim_ratio_impl<R1>::type;

    template <RatioType R1 = Rational<1, 1>>
    struct temp_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_temp, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using temp_dim_ratio = typename temp_dim_ratio_impl<R1>::type;

    template <RatioType R1 = Rational<1, 1>>
    struct amount_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_amount, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using amount_dim_ratio = typename amount_dim_ratio_impl<R1>::type;

    template <RatioType R1 = Rational<1, 1>>
    struct luminous_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_luminous, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using luminous_dim_ratio = typename luminous_dim_ratio_impl<R1>::type;

    template <RatioType R1 = Rational<1, 1>>
    struct angle_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_angle, R1>;
    };

    template <RatioType R1 = Rational<1, 1>>
    using angle_dim_ratio = typename angle_dim_ratio_impl<R1>::type;
}   // namespace mstd

#endif   // __MSTD_DIM_RATIO_IMPL_HPP__