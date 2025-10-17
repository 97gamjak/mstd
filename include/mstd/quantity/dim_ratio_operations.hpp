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

#ifndef __MSTD_DIM_RATIO_OPERATIONS_HPP__
#define __MSTD_DIM_RATIO_OPERATIONS_HPP__

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
    template <DimRatioType R1, DimType D1, DimRatioType R2, DimType D2>
    using dim_ratio_mul_t = details::dim_ratio_mul_impl<R1, D1, R2, D2>::type;

    /**
     * @brief DimRatio division
     *
     * @tparam R1
     * @tparam R2
     * @return the resulting DimRatio after division
     */
    template <DimRatioType R1, DimRatioType R2>
    using dim_ratio_div_t = DimRatio<
        ratio_pack_div_t<typename R1::si, typename R2::si>,
        ratio_pack_div_t<typename R1::ex, typename R2::ex>>;

    /**
     * @brief DimRatio power
     *
     * @tparam DimRatio
     * @tparam Exp
     * @return the resulting DimRatio after power
     */
    template <DimRatioType R, DimType D>
    using dim_ratio_pow_t = details::dim_ratio_pow_impl<R, D>::type;

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
    template <details::SimpleDim Dim, RatioType Ratio>
    using make_dim_ratio_single_t =
        typename details::make_dim_ratio_single<Dim, Ratio>::type;

}   // namespace mstd

#endif   // __MSTD_DIM_RATIO_OPERATIONS_HPP__