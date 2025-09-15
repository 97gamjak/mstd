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

#include "dim_ratio.hpp"
#include "dimension.hpp"
#include "mstd/ratio.hpp"

namespace mstd::units
{
    template <
        class Dim,
        class RatioDim,
        ratio::StdRatio GlobalRatio = std::ratio<1>>
    struct unit
    {
        // TODO: implement false type and custom MACRO
        static_assert(std::false_type::value, "unit must be specialized");
    };

    template <class Dim, class RatioDim, ratio::StdRatio GlobalRatio>
    requires(is_dim_ratio_v<RatioDim>)
    struct unit<Dim, RatioDim, GlobalRatio>
    {
        using dim    = Dim;
        using ratio  = RatioDim;
        using global = GlobalRatio;
    };

    template <
        SimpleBaseDim   Dim,
        ratio::StdRatio Ratio,
        ratio::StdRatio GlobalRatio>
    struct unit<Dim, Ratio, GlobalRatio>
    {
        using dim    = Dim;
        using ratio  = make_dim_ratio_single_t<Dim, Ratio>;
        using global = GlobalRatio;
    };

    template <class Dim, ratio::StdRatio GlobalRatio>
    struct unit<Dim, std::ratio<1>, GlobalRatio>
    {
        using dim    = Dim;
        using ratio  = dim_ratio<>;
        using global = GlobalRatio;
    };

    template <
        class Dim,
        long double F,
        class DimRatioOrRatio,
        ratio::StdRatio GlobalRatio = std::ratio<1>>
    struct real_unit : unit<Dim, DimRatioOrRatio, GlobalRatio>
    {
        static constexpr long double factor = F;
    };

}   // namespace mstd::units

#endif   // __MSTD_UNITS_UNIT_HPP__