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

#ifndef __MSTD__QUANTITY__QUANTITY_IMPL_HPP__
#define __MSTD__QUANTITY__QUANTITY_IMPL_HPP__

#include "mstd/error.hpp"

MSTD_WARN_BUGGY_HEADER("mstd/quantity/quantity_impl.hpp")

#include "quantity.hpp"
#include "unit_impl.hpp"

namespace mstd
{
    /***************************
     *                         *
     * Common quantity aliases *
     *                         *
     ***************************/

    template <length_unit U, class Rep = double>
    using Length = Quantity<U, Rep>;

    template <mass_unit U, class Rep = double>
    using Mass = Quantity<U, Rep>;

    template <time_unit U, class Rep = double>
    using Time = Quantity<U, Rep>;

    template <current_unit U, class Rep = double>
    using Current = Quantity<U, Rep>;

    template <temperature_unit U, class Rep = double>
    using Temperature = Quantity<U, Rep>;

    template <amount_unit U, class Rep = double>
    using Amount = Quantity<U, Rep>;

    template <luminous_unit U, class Rep = double>
    using LuminousIntensity = Quantity<U, Rep>;

    template <angle_unit U, class Rep = double>
    using Angle = Quantity<U, Rep>;

    template <currency_unit U, class Rep = double>
    using Currency = Quantity<U, Rep>;

    template <info_unit U, class Rep = double>
    using Info = Quantity<U, Rep>;

    template <scalar_unit U, class Rep = double>
    using Scalar = Quantity<U, Rep>;

    template <dimensionless_unit U, class Rep = double>
    using Dimensionless = Quantity<U, Rep>;

    template <area_unit U, class Rep = double>
    using Area = Quantity<U, Rep>;

    template <volume_unit U, class Rep = double>
    using Volume = Quantity<U, Rep>;

    template <density_unit U, class Rep = double>
    using Density = Quantity<U, Rep>;

    template <velocity_unit U, class Rep = double>
    using Velocity = Quantity<U, Rep>;

    template <acceleration_unit U, class Rep = double>
    using Acceleration = Quantity<U, Rep>;

    template <force_unit U, class Rep = double>
    using Force = Quantity<U, Rep>;

    template <energy_unit U, class Rep = double>
    using Energy = Quantity<U, Rep>;

}   // namespace mstd

#endif   // __MSTD__QUANTITY__QUANTITY_IMPL_HPP__