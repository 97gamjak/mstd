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
#include "mstd/math.hpp"
#include "mstd/ratio.hpp"
#include "mstd/type_traits.hpp"

namespace mstd
{
    /**
     * @brief placeholder for a Unit
     *
     * @tparam DimRatio The DimRatio of the unit (default: all PowRatio<>)
     * @tparam Global The global ratio of the unit (default: 1/1)
     * @tparam Exp The exponent of the unit (default: Rational<1,1>)
     * @tparam F The real factor of the unit (default: 1.0L)
     */
    template <
        DimRatioType DimRatio = DimRatio<>,
        RatioType    Global   = ratio<1>,
        RationalType Exp      = Rational<1, 1>,
        long double  F        = 1.0L>
    struct Unit
    {
        using dimRatio                        = DimRatio;
        using global                          = Global;
        using exponent                        = Exp;
        static constexpr long double factor_v = F;
    };

}   // namespace mstd

#endif   // __MSTD_UNITS_UNIT_HPP__