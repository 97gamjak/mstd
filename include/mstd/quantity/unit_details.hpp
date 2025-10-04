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
#include "mstd/math.hpp"
#include "mstd/ratio.hpp"
#include "unit.hpp"

/**
 * @file unit_details.hpp
 * @brief Internal traits and implementations for unit algebra.
 *
 * Provides trait extraction for unit types and the low-level implementations
 * used to multiply, divide, and exponentiate units, including handling of
 * real scaling factors and ratio/global-ratio composition.
 */

namespace mstd::units::details
{
    /*****************
     *               *
     * Unit traits   *
     *               *
     *****************/

    /**
     * @brief Trait carrier for unit-like types.
     *
     * Primary template left undefined; only specific unit forms are
     * specialized below to expose `dim`, `ratio`, `global`, `factor`, and
     * whether a type is a real unit (`is_real`).
     */
    template <class U>
    struct unit_traits;

    /**
     * @brief Traits for `unit<Dim, Ratio, GlobalRatio>`.
     */
    template <class Dim, class Ratio, ratio::StdRatio GlobalRatio>
    struct unit_traits<unit<Dim, Ratio, GlobalRatio>>
    {
        using dim    = Dim;
        using ratio  = typename unit<Dim, Ratio, GlobalRatio>::ratio;
        using global = GlobalRatio;
        static constexpr long double factor  = 1.0L;
        static constexpr bool        is_real = false;
    };

    /**
     * @brief Traits for `real_unit<Dim, F, Ratio, GlobalRatio>`.
     */
    template <
        class Dim,
        long double F,
        class Ratio,
        ratio::StdRatio GlobalRatio>
    struct unit_traits<real_unit<Dim, F, Ratio, GlobalRatio>>
    {
        using dim    = Dim;
        using ratio  = typename real_unit<Dim, F, Ratio, GlobalRatio>::ratio;
        using global = GlobalRatio;
        static constexpr long double factor  = F;
        static constexpr bool        is_real = true;
    };

    /*************************
     *                       *
     * Build unit selection  *
     *                       *
     *************************/

    /**
     * @brief Selects `unit` vs `real_unit` based on factor and flags.
     *
     * If `AnyReal` is true or `FactorToSI != 1`, the resulting type is a
     * `real_unit`; otherwise it is a plain `unit`.
     */
    template <
        class Dim,
        long double FactorToSI,
        class Ratio,
        ratio::StdRatio GlobalRatio,
        bool            AnyReal>
    struct build_unit_impl;

    template <
        class Dim,
        long double F,
        class Ratio,
        ratio::StdRatio GlobalRatio>
    struct build_unit_impl<Dim, F, Ratio, GlobalRatio, false>
    {
        using type = unit<Dim, Ratio, GlobalRatio>;
    };

    template <
        class Dim,
        long double F,
        class Ratio,
        ratio::StdRatio GlobalRatio>
    struct build_unit_impl<Dim, F, Ratio, GlobalRatio, true>
    {
        using type = real_unit<Dim, F, Ratio, GlobalRatio>;
    };

    template <
        class Dim,
        long double FactorToSI,
        class Ratio,
        ratio::StdRatio GlobalRatio,
        bool            AnyReal>
    using build_unit_t = typename build_unit_impl<
        Dim,
        FactorToSI,
        Ratio,
        GlobalRatio,
        (AnyReal || (FactorToSI != static_cast<long double>(1)))>::type;

    /************************
     *                      *
     * Operation back-ends  *
     *                      *
     ************************/

    /**
     * @brief Implementation of unit multiplication at type level.
     *
     * Multiplies dimensions, composes dim ratios and global ratios, and
     * multiplies real factors. Chooses `unit` or `real_unit` accordingly.
     */
    template <class Unit1, class Unit2>
    struct unit_mul_impl
    {
        using T1    = unit_traits<Unit1>;
        using T2    = unit_traits<Unit2>;
        using dim   = dim_mul_t<typename T1::dim, typename T2::dim>;
        using ratio = dim_ratio_mul_t<typename T1::ratio, typename T2::ratio>;
        using global =
            std::ratio_multiply<typename T1::global, typename T2::global>;

        static constexpr long double f        = T1::factor * T2::factor;
        static constexpr bool        any_real = T1::is_real || T2::is_real;
        using type = build_unit_t<dim, f, ratio, global, any_real>;
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
    template <class Unit1, class Unit2>
    struct unit_div_impl
    {
        using T1    = unit_traits<Unit1>;
        using T2    = unit_traits<Unit2>;
        using dim   = dim_div_t<typename T1::dim, typename T2::dim>;
        using ratio = dim_ratio_div_t<typename T1::ratio, typename T2::ratio>;
        using global =
            std::ratio_divide<typename T1::global, typename T2::global>;

        static constexpr long double factor   = T1::factor / T2::factor;
        static constexpr bool        any_real = T1::is_real || T2::is_real;
        using type = build_unit_t<dim, factor, ratio, global, any_real>;
    };

    /**
     * @brief Implementation of raising a unit to an integer power.
     *
     * Exponentiates dimensions and ratios; raises the real factor; preserves
     * real-ness if the base unit is real.
     */
    template <class Unit, int Exp>
    struct unit_pow_impl
    {
        using T      = unit_traits<Unit>;
        using dim    = dim_pow_t<typename T::dim, Exp>;
        using ratio  = dim_ratio_pow_t<typename T::ratio, Exp>;
        using global = ratio::template ratio_pow_t<typename T::global, Exp>;
        static constexpr long double factor   = math::power(T::factor, Exp);
        static constexpr bool        any_real = T::is_real;
        using type = build_unit_t<dim, factor, ratio, global, any_real>;
    };

    template <class Unit, class Dim>
    inline constexpr bool has_dim_v =
        std::is_same_v<typename unit_traits<Unit>::dim, Dim>;

}   // namespace mstd::units::details

#endif   // __MSTD_UNITS_DETAILS_HPP__
