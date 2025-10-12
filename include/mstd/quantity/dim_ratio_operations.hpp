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
    template <DimRatioType R1, DimRatioType R2>
    using dim_ratio_mul_t = DimRatio<
        ratio_pack_mul_t<typename R1::si, typename R2::si>,
        ratio_pack_mul_t<typename R1::ex, typename R2::ex>>;

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
    template <DimRatioType R, int Exp>
    using dim_ratio_pow_t = DimRatio<
        ratio_pack_pow_t<typename R::si, Exp>,
        ratio_pack_pow_t<typename R::ex, Exp>>;

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