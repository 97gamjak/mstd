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

#include "mstd/pack.hpp"
#include "mstd/ratio.hpp"
#include "unit_details.hpp"

/**
 * @file unit_operations.hpp
 * @brief Compile-time utilities for unit algebra and scaling.
 *
 * Provides helpers to combine units (multiply, divide, power), check
 * dimensional compatibility, and compute conversion/scaling factors
 * relative to SI from a unit's ratio, global ratio, and real factor.
 */

namespace mstd::units
{

    /**
     * @brief Checks if two units have the same dimension
     *
     * @tparam Unit1
     * @tparam Unit2
     */
    template <class Unit1, class Unit2>
    inline constexpr bool same_dimension_v = std::is_same_v<
        typename details::unit_traits<Unit1>::dim,
        typename details::unit_traits<Unit2>::dim>;

    /**
     * @brief
     *
     *
     * @tparam Unit1
     * @tparam Unit2
     */
    template <class Unit1, class Unit2>
    inline constexpr bool compatible_units_v = same_dimension_v<Unit1, Unit2>;

    /**
     * @brief Multiply two units to form a composite unit.
     *
     * @tparam Unit1 Left unit
     * @tparam Unit2 Right unit
     */
    template <class Unit1, class Unit2>
    using unit_mul = typename details::unit_mul_impl<Unit1, Unit2>::type;

    /**
     * @brief Divide two units to form a composite unit.
     *
     * @tparam Unit1 Numerator unit
     * @tparam Unit2 Denominator unit
     */
    template <class Unit1, class Unit2>
    using unit_div = typename details::unit_div_impl<Unit1, Unit2>::type;

    /**
     * @brief Raise a unit to an integer power.
     *
     * @tparam Unit Base unit
     * @tparam Exp  Integer exponent (can be negative)
     */
    template <class Unit, int Exp>
    using unit_pow = typename details::unit_pow_impl<Unit, Exp>::type;

    /**
     * @brief Compile-time ratio multiplier of a unit relative to SI.
     *
     * Product of the dimensional ratio components (SI and extra ratio pack)
     * and the unit's global ratio.
     *
     * @tparam Unit Unit to query
     */
    template <class Unit>
    inline constexpr long double ratio_v =
        pack::details::ratio_pack_v<
            typename details::unit_traits<Unit>::ratio::si> *
        pack::details::ratio_pack_v<
            typename details::unit_traits<Unit>::ratio::ex> *
        ratio::ratio_v<typename details::unit_traits<Unit>::global>;

    /**
     * @brief Real scaling factor attached to a unit.
     *
     * For regular units this is 1; for real_unit it is the provided factor.
     *
     * @tparam Unit Unit to query
     */
    template <class Unit>
    inline constexpr long double factor_v = details::unit_traits<Unit>::factor;

    /**
     * @brief Overall scale of a unit relative to SI.
     *
     * Equals `factor_v<Unit> * ratio_v<Unit>` and can be used to convert
     * between the unit and its SI counterpart.
     *
     * @tparam Unit Unit to query
     */
    template <class Unit>
    inline constexpr long double scale_v = factor_v<Unit> * ratio_v<Unit>;

}   // namespace mstd::units

#endif   // __MSTD_UNIT_OPERATIONS_HPP__
