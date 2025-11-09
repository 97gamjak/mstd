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

#ifndef __MSTD_UNIT_OPERATIONS_HPP__
#define __MSTD_UNIT_OPERATIONS_HPP__

#include "mstd/math.hpp"
#include "mstd/pack.hpp"
#include "unit_details.hpp"

/**
 * @file unit_operations.hpp
 * @brief Compile-time utilities for unit algebra and scaling.
 *
 * Provides helpers to combine units (multiply, divide, power), check
 * dimensional compatibility, and compute conversion/scaling factors
 * relative to SI from a unit's ratio, global ratio, and real factor_v.
 */

namespace mstd
{
    template <DimType D>
    using build_unit_t = typename details::build_unit_impl<D>::type;

    /**
     * @brief Multiply two units to form a composite unit.
     *
     * @tparam Unit1 First unit
     * @tparam Unit2 Second unit
     * @tparam Units Additional units to multiply (optional)
     */
    template <UnitType... Units>
    using unit_mul_t = typename details::unit_mul_pack<Units...>::type;

    /**
     * @brief Divide two units to form a composite unit.
     *
     * @tparam Unit1 Numerator unit
     * @tparam Unit2 Denominator unit
     * @tparam Units Additional units to multiply into the denominator
     * (optional)
     */
    template <UnitType Unit1, UnitType Unit2, UnitType... Units>
    using unit_div_t = typename details::
        unit_div_impl<Unit1, unit_mul_t<Unit2, Units...>>::type;

    /**
     * @brief Raise a unit to an integer power.
     *
     * @tparam Unit Base unit
     * @tparam Exp  Integer exponent (can be negative)
     */
    template <UnitType Unit, int Exp>
    using unit_pow_t = typename details::unit_pow_impl<Unit, Exp>::type;

    /**
     * @brief Compile-time ratio multiplier of a unit relative to SI.
     *
     * Product of the dimensional ratio components (SI and extra ratio pack)
     * and the unit's global ratio.
     *
     * @details this is a partial specialization
     *
     * @tparam Unit Unit to query
     */
    template <UnitType Unit>
    struct ratio_v_impl<Unit>
    {
        static constexpr long double value =
            ratio_v<typename Unit::dimRatio::si> *
            ratio_v<typename Unit::dimRatio::ex> *
            ratio_v<typename Unit::global>;
    };

    /**
     * @brief Real scaling factor attached to a unit.
     *
     * For regular units this is 1; for real_unit it is the provided factor.
     *
     * @tparam Unit Unit to query
     */
    template <UnitType Unit>
    inline constexpr long double factor_v = Unit::factor_v;

    /**
     * @brief Overall scale of a unit relative to SI.
     *
     * Equals `factor_v<Unit> * ratio_v<Unit>` and can be used to convert
     * between the unit and its SI counterpart.
     *
     * @tparam Unit Unit to query
     */
    template <UnitType Unit>
    inline constexpr long double scale_v = factor_v<Unit> * ratio_v<Unit>;

    /**
     * @brief Scale a unit by a real factor at compile time.
     *
     * Produces a new unit with the same dimension and ratios, but with the
     * provided real factor applied.
     *
     * @tparam Unit Base unit to scale
     * @tparam F Scaling factor (must be non-zero)
     */
    template <UnitType Unit, long double F>
    using scaled_unit_t = typename details::scaled_unit_impl<Unit, F>::type;

    /**
     * @brief Get the common unit type for two compatible units.
     *
     * @tparam U1 The first unit type.
     * @tparam U2 The second unit type.
     */
    template <UnitType U1, UnitType U2>
    using common_unit_t = typename details::common_unit_impl<U1, U2>::type;

}   // namespace mstd

#endif   // __MSTD_UNIT_OPERATIONS_HPP__
