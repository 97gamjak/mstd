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

#ifndef __MSTD_UNITS_UNIT_HPP__
#define __MSTD_UNITS_UNIT_HPP__

#include "dim_details.hpp"
#include "dim_ratio.hpp"
#include "mstd/error.hpp"
#include "mstd/ratio.hpp"
#include "mstd/tags/unit_tag.hpp"

namespace mstd::units
{
    /**
     * @brief placeholder for a unit
     *
     * @details
     *
     * This is a placeholder for a unit. The general case is forbidden.
     * All applicable specializations must be provided explicitly.
     *
     * @tparam Dim
     * @tparam RatioDim
     * @tparam GlobalRatio
     */
    template <
        typename Dim,
        typename RatioDim,
        ratio::StdRatio GlobalRatio = std::ratio<1>>
    struct unit : public unit_tag
    {
        MSTD_COMPILE_FAIL("unit must be specialized");
    };

    /**
     * @brief unit definition for a dimension, a dim_ratio and a ratio as a
     * global ratio
     *
     * @tparam Dim
     * @tparam RatioDim
     * @tparam GlobalRatio
     */
    template <
        details::DimType  Dim,
        details::DimRatio RatioDim,
        ratio::StdRatio   GlobalRatio>
    struct unit<Dim, RatioDim, GlobalRatio> : public unit_tag
    {
        using dim    = Dim;
        using ratio  = RatioDim;
        using global = GlobalRatio;
    };

    /**
     * @brief unit definition for a dimension (a simple base dimension), a ratio
     * and a ratio as a global ratio
     *
     * @tparam Dim
     * @tparam Ratio
     * @tparam GlobalRatio
     */
    template <
        details::SimpleDim Dim,
        ratio::StdRatio    Ratio,
        ratio::StdRatio    GlobalRatio>
    struct unit<Dim, Ratio, GlobalRatio> : public unit_tag
    {
        using dim    = Dim;
        using ratio  = make_dim_ratio_single_t<Dim, Ratio>;
        using global = GlobalRatio;
    };

    /**
     * @brief unit definition for a dimension (a simple base dimension), no
     * ratio and a ratio as a global ratio
     *
     * @tparam Dim
     * @tparam GlobalRatio
     */
    template <details::DimType Dim, ratio::StdRatio GlobalRatio>
    struct unit<Dim, std::ratio<1>, GlobalRatio> : public unit_tag
    {
        using dim    = Dim;
        using ratio  = dim_ratio<>;
        using global = GlobalRatio;
    };

    /**
     * @brief real unit definition. This is a unit with a real factor as a
     * general multiplier/scaling factor
     *
     * @tparam Dim
     * @tparam Ratio
     * @tparam GlobalRatio
     */
    template <
        details::DimType Dim,
        long double      F,
        typename DimRatioOrRatio,
        ratio::StdRatio GlobalRatio = std::ratio<1>>
    requires(
        ratio::StdRatio<DimRatioOrRatio> || details::DimRatio<DimRatioOrRatio>
    )
    struct real_unit : unit<Dim, DimRatioOrRatio, GlobalRatio>
    {
        static constexpr long double factor = F;
    };

}   // namespace mstd::units

#endif   // __MSTD_UNITS_UNIT_HPP__