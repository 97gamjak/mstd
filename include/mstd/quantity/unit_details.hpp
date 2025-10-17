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

#ifndef __MSTD_UNITS_DETAILS_HPP__
#define __MSTD_UNITS_DETAILS_HPP__

#include "dim.hpp"
#include "dim_ratio_operations.hpp"
#include "mstd/math.hpp"
#include "mstd/ratio.hpp"
#include "mstd/type_traits/quantity_traits.hpp"
#include "unit.hpp"

/**
 * @file unit_details.hpp
 * @brief Internal traits and implementations for unit algebra.
 *
 * Provides trait extraction for unit types and the low-level implementations
 * used to multiply, divide, and exponentiate units, including handling of
 * real scaling factors and ratio/global-ratio composition.
 */

namespace mstd::details
{
    /*******************************************************************
     *                                                                 *
     * multiplying, dividing, powering and scaling units at type level *
     *                                                                 *
     *******************************************************************/

    /**
     * @brief Implementation of unit multiplication at type level.
     *
     * Multiplies dimensions, composes dim ratios and global ratios, and
     * multiplies real factors. Chooses `unit` or `real_unit` accordingly.
     */
    template <class U1, class U2>
    struct unit_mul_impl
    {
        using _D1    = typename U1::dim;
        using _D2    = typename U2::dim;
        using _R1    = typename U1::ratio;
        using _R2    = typename U2::ratio;
        using dim    = dim_mul_t<_D1, _D2>;
        using ratio  = dim_ratio_mul_t<_R1, _D1, _R2, _D2>;
        using global = ratio_mul_t<typename U1::global, typename U2::global>;

        static constexpr long double f = U1::factor_v * U2::factor_v;
        using type                     = Unit<dim, ratio, global, f>;
    };

    /**
     * @brief Helper struct to multiply multiple units.
     *
     * Recursively reduces the parameter pack by multiplying the first two
     * units, then combining the result with the next unit, and so on.
     */
    template <class... Units>
    struct unit_mul_pack;

    /** @brief Base case: no units provided.
     *
     * This specialization triggers a static assertion failure to ensure that
     * at least one unit type is provided.
     */
    template <>
    struct unit_mul_pack<>
    {
        static_assert(
            std::is_void_v<void>,
            "unit_mul_pack<>: at least one unit type must be provided"
        );
    };

    /** @brief Base case: single unit provided.
     *
     * This specialization simply returns the provided unit type.
     */
    template <class U>
    struct unit_mul_pack<U>
    {
        using type = U;
    };

    /**
     * @brief Implementation of unit multiplication for multiple units.
     *
     * Recursively reduces the parameter pack by multiplying the first two
     * units, then combining the result with the next unit, and so on.
     */
    template <class U1, class U2, class... Us>
    struct unit_mul_pack<U1, U2, Us...>
    {
        using type = typename unit_mul_pack<
            typename unit_mul_impl<U1, U2>::type,
            Us...>::type;
    };

    /**
     * @brief Implementation of unit division at type level.
     *
     * Divides dimensions, composes dim ratios and global ratios, and divides
     * real factors. Chooses `unit` or `real_unit` accordingly.
     */
    template <class U1, class U2>
    struct unit_div_impl
    {
        using dim    = dim_div_t<typename U1::dim, typename U2::dim>;
        using ratio  = dim_ratio_div_t<typename U1::ratio, typename U2::ratio>;
        using global = ratio_div_t<typename U1::global, typename U2::global>;

        static constexpr long double factor_v = U1::factor_v / U2::factor_v;

        using type = Unit<dim, ratio, global, factor_v>;
    };

    /**
     * @brief Implementation of raising a unit to an integer power.
     *
     * Exponentiates dimensions and ratios; raises the real factor; preserves
     * real-ness if the base unit is real.
     */
    template <class U, int Exp>
    struct unit_pow_impl
    {
        using dim    = dim_pow_t<typename U::dim, Exp>;
        using ratio  = dim_ratio_pow_k_t<typename U::ratio, Exp>;
        using global = ratio_pow_t<typename U::global, Exp>;

        static constexpr long double factor_v = power(U::factor_v, Exp);

        using type = Unit<dim, ratio, global, factor_v>;
    };

    /**
     * @brief Extract the real scaling factor of a unit relative to SI.
     *
     * Product of the dimensional ratio components (SI and extra ratio pack)
     * and the unit's global ratio.
     */
    template <class U, long double F>
    struct scaled_unit_impl
    {
        using type = Unit<
            typename U::dim,
            typename U::ratio,
            typename U::global,
            U::factor_v * F>;
    };

    /**************************************************
     *                                                *
     * common unit selection for two compatible units *
     *                                                *
     **************************************************/

    /**
     * @brief Get the common unit type for two compatible units.
     *
     * @tparam U1 The first unit type.
     * @tparam U2 The second unit type.
     * @tparam Enable SFINAE helper.
     */
    template <UnitType U1, UnitType U2, class Enable = void>
    struct common_unit_impl;

    /**
     * @brief Get the common unit type for two compatible units.
     *
     * @details The common unit is determined by the following rules:
     * 1. if only one has factor==1, pick that one
     * 2. if both factors != 1, pick LHS
     * 3. both factors == 1 → pick smallest ratio (finer); tie → LHS
     *
     * @tparam Unit1 The first unit type.
     * @tparam Unit2 The second unit type.
     */
    template <UnitType Unit1, UnitType Unit2>
    struct common_unit_impl<
        Unit1,
        Unit2,
        std::enable_if_t<same_dimension_v<Unit1, Unit2>>>
    {
        static constexpr long double f1   = Unit1::factor_v;
        static constexpr long double f2   = Unit2::factor_v;
        static constexpr bool        one1 = (f1 == 1.0L);
        static constexpr bool        one2 = (f2 == 1.0L);

        static constexpr bool only1 = (one1 && !one2);
        static constexpr bool only2 = (!one1 && one2);
        static constexpr bool both  = (one1 && one2);
        static constexpr bool none  = (!one1 && !one2);

        // rule 1: if only one has factor==1, pick that one
        using rule1 = std::
            conditional_t<only1, Unit1, std::conditional_t<only2, Unit2, void>>;

        // rule 2: if both factors != 1, pick LHS
        using rule2 = std::conditional_t<none, Unit1, void>;

        // rule 3: both factors == 1 → pick smallest ratio (finer); tie → LHS
        static constexpr long double r1 = ratio_v<Unit1>;
        static constexpr long double r2 = ratio_v<Unit2>;

        using rule3 = std::conditional_t<
            both,
            std::conditional_t<
                (r1 < r2),
                Unit1,
                std::conditional_t<(r2 < r1), Unit2, Unit1>>,   // tie → LHS
            void>;

        // select type based on rules
        using type = std::conditional_t<
            !std::is_void_v<rule1>,
            rule1,
            std::conditional_t<!std::is_void_v<rule2>, rule2, rule3>>;
    };

}   // namespace mstd::details

#endif   // __MSTD_UNITS_DETAILS_HPP__
