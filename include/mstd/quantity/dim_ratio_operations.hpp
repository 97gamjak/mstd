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

#ifndef __MSTD__QUANTITY__DIM_RATIO_OPERATIONS_HPP__
#define __MSTD__QUANTITY__DIM_RATIO_OPERATIONS_HPP__

#include "dim_ratio.hpp"
#include "dim_ratio_details.hpp"

namespace mstd
{
    /**
     * @brief DimRatio multiplication
     *
     * @tparam Ratio1
     * @tparam Ratio2
     * @return the resulting DimRatio after multiplication
     */
    template <DimRatioType R1, DimRatioType R2>
    struct mul_type<R1, R2>
    {
        MSTD_COMPILE_FAIL(
            "There is no standard mul_type_t or mul_type possible for DimRatio"
            "Please consider using dim_ratio_mul_type instead."
        );
    };

    /**
     * @brief DimRatio division
     *
     * @tparam R1
     * @tparam R2
     * @return the resulting DimRatio after division
     */
    template <DimRatioType R1, DimRatioType R2>
    struct div_type<R1, R2>
    {
        MSTD_COMPILE_FAIL(
            "There is no standard div_type_t or div_type possible for DimRatio"
            "Please consider using dim_ratio_div_type instead."
        );
    };

    template <
        RationalPowerType R1,
        RationalType      D1,
        RationalPowerType R2,
        RationalType      D2>
    struct dim_ratio_mul_type_entry
    {
        using mul      = mul_type_t<R1, R2>;
        using mul_base = mul::base;
        using mul_exp  = mul::exp;
        using exp      = add_type_t<D1, D2>;
        using R1_base  = R1::base;

        using remaining_exp   = sub_type_t<exp, mul_exp>;
        using remaining_base1 = div_type_t<mul_base, R1_base>;

        using remainder = mul_type_t<
            RationalPower<remaining_base1, exp>,
            RationalPower<mul_base, remaining_exp>>;
        using type = RationalPower<mul_base, exp>;
    };

    template <
        RationalPowerType R1,
        RationalType      D1,
        RationalPowerType R2,
        RationalType      D2>
    struct dim_ratio_div_type_entry
    {
        using div      = div_type<R1, R2>;
        using div_base = div::base;
        using div_exp  = div::exp;
        using exp      = sub_type_t<D1, D2>;
        using R1_base  = R1::base;

        using remaining_exp   = sub_type_t<exp, div_exp>;
        using remaining_base1 = div_type_t<div_base, R1_base>;

        using remainder = div_type_t<
            RationalPower<remaining_base1, exp>,
            RationalPower<div_base, remaining_exp>>;
        using type = RationalPower<div_base, exp>;
    };

    template <
        RationalPowerType R1,
        RationalType      D1,
        RationalPowerType R2,
        RationalType      D2>
    using dimRatioMul_t =
        typename dim_ratio_mul_type_entry<R1, D1, R2, D2>::type;

    template <
        RationalPowerType R1,
        RationalType      D1,
        RationalPowerType R2,
        RationalType      D2>
    using dimRatioMul_remainder_t =
        typename dim_ratio_mul_type_entry<R1, D1, R2, D2>::remainder;

    template <
        RationalPowerType R1,
        RationalType      D1,
        RationalPowerType R2,
        RationalType      D2>
    using dimRatioDiv_t =
        typename dim_ratio_div_type_entry<R1, D1, R2, D2>::type;

    template <
        RationalPowerType R1,
        RationalType      D1,
        RationalPowerType R2,
        RationalType      D2>
    using dimRatioDiv_remainder_t =
        typename dim_ratio_div_type_entry<R1, D1, R2, D2>::remainder;

    template <DimRatioType R1, DimType D1, DimRatioType R2, DimType D2>
    struct dim_ratio_mul_type
    {
        using ratio_s1 = R1::si;
        using ratio_e1 = R1::ex;
        using ratio_s2 = R2::si;
        using ratio_e2 = R2::ex;
        using dim_s1   = D1::si;
        using dim_e1   = D1::ex;
        using dim_s2   = D2::si;
        using dim_e2   = D2::ex;

        using type_si =
            zip_type_t<dimRatioMul_t, ratio_s1, dim_s1, ratio_s2, dim_s2>;
        using type_ex =
            zip_type_t<dimRatioMul_t, ratio_e1, dim_e1, ratio_e2, dim_e2>;
        using type = DimRatio<type_si, type_ex>;

        using remainder_pack_si = zip_type_t<
            dimRatioMul_remainder_t,
            ratio_s1,
            dim_s1,
            ratio_s2,
            dim_s2>;
        using remainder_pack_ex = zip_type_t<
            dimRatioMul_remainder_t,
            ratio_e1,
            dim_e1,
            ratio_e2,
            dim_e2>;
        using remainder = mul_type_t<
            reduce_type_t<mul_type_t, RatioPower<>, remainder_pack_si>,
            reduce_type_t<mul_type_t, RatioPower<>, remainder_pack_ex>>;
    };

    template <DimRatioType R1, DimType D1, DimRatioType R2, DimType D2>
    struct dim_ratio_div_type
    {
        using ratio_s1 = R1::si;
        using ratio_e1 = R1::ex;
        using ratio_s2 = R2::si;
        using ratio_e2 = R2::ex;
        using dim_s1   = D1::si;
        using dim_e1   = D1::ex;
        using dim_s2   = D2::si;
        using dim_e2   = D2::ex;

        using type_si =
            zip_type_t<dimRatioDiv_t, ratio_s1, dim_s1, ratio_s2, dim_s2>;
        using type_ex =
            zip_type_t<dimRatioDiv_t, ratio_e1, dim_e1, ratio_e2, dim_e2>;
        using type = DimRatio<type_si, type_ex>;

        using remainder_pack_si = zip_type_t<
            dimRatioDiv_remainder_t,
            ratio_s1,
            dim_s1,
            ratio_s2,
            dim_s2>;
        using remainder_pack_ex = zip_type_t<
            dimRatioDiv_remainder_t,
            ratio_e1,
            dim_e1,
            ratio_e2,
            dim_e2>;
        using remainder = mul_type_t<
            reduce_type_t<mul_type_t, RatioPower<>, remainder_pack_si>,
            reduce_type_t<mul_type_t, RatioPower<>, remainder_pack_ex>>;
    };

    /**
     * @brief DimRatio power
     *
     * @tparam DimRatio
     * @tparam Exp
     * @return the resulting DimRatio after power
     */
    template <DimRatioType R, DimType D>
    struct pow_type<R, D>
    {
        using type = details::dim_ratio_pow_impl<R, D>::type;
    };

    /**
     * @brief DimRatio power with integer K
     *
     * @tparam DimRatio
     * @tparam K
     * @return the resulting DimRatio after power
     */
    template <DimRatioType R, int K>
    using dim_ratio_pow_k_t = DimRatio<
        ratio_pack_pow_k_t<typename R::si, K>,
        ratio_pack_pow_k_t<typename R::ex, K>>;

    /***************************************
     *                                     *
     * Build ratio_dim from a single ratio *
     *                                     *
     ***************************************/

    /**
     * @brief make a DimRatio from a dimension and a single ratio
     *
     * @tparam D
     * @tparam R
     * @return the resulting DimRatio
     */
    template <details::SimpleDim Dim, RationalPowerType Ratio>
    using make_dim_ratio_single_t =
        typename details::make_dim_ratio_single<Dim, Ratio>::type;

}   // namespace mstd

#endif   // __MSTD__QUANTITY__DIM_RATIO_OPERATIONS_HPP__