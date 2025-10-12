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

namespace mstd
{
    template <details::DimType Dim>
    struct BaseUnit : public unit_tag
    {
        using dim                           = Dim;
        static constexpr long double factor = 1.0L;
    };

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
        details::DimType  Dim,
        details::DimRatio RatioDim    = dim_ratio<>,
        StdRatio          GlobalRatio = std::ratio<1>,
        long double       F           = 1.0L>
    struct unit : public BaseUnit<Dim>
    {
        using ratio                         = RatioDim;
        using global                        = GlobalRatio;
        static constexpr long double factor = F;
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
        StdRatio          GlobalRatio>
    struct unit<Dim, RatioDim, GlobalRatio> : public BaseUnit<Dim>
    {
        using ratio  = RatioDim;
        using global = GlobalRatio;
    };

}   // namespace mstd

#endif   // __MSTD_UNITS_UNIT_HPP__