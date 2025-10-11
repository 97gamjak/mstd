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

#ifndef __MSTD_UNITS_QUANTITY_IMPL_HPP__
#define __MSTD_UNITS_QUANTITY_IMPL_HPP__

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
    using Length = quantity<U, Rep>;

    template <mass_unit U, class Rep = double>
    using Mass = quantity<U, Rep>;

    template <time_unit U, class Rep = double>
    using Time = quantity<U, Rep>;

    template <current_unit U, class Rep = double>
    using Current = quantity<U, Rep>;

    template <temperature_unit U, class Rep = double>
    using Temperature = quantity<U, Rep>;

    template <amount_unit U, class Rep = double>
    using Amount = quantity<U, Rep>;

    template <luminous_unit U, class Rep = double>
    using LuminousIntensity = quantity<U, Rep>;

    template <angle_unit U, class Rep = double>
    using Angle = quantity<U, Rep>;

    template <currency_unit U, class Rep = double>
    using Currency = quantity<U, Rep>;

    template <info_unit U, class Rep = double>
    using Info = quantity<U, Rep>;

    template <scalar_unit U, class Rep = double>
    using Scalar = quantity<U, Rep>;

    template <dimensionless_unit U, class Rep = double>
    using Dimensionless = quantity<U, Rep>;

    template <area_unit U, class Rep = double>
    using Area = quantity<U, Rep>;

    template <volume_unit U, class Rep = double>
    using Volume = quantity<U, Rep>;

    template <density_unit U, class Rep = double>
    using Density = quantity<U, Rep>;

    template <velocity_unit U, class Rep = double>
    using Velocity = quantity<U, Rep>;

    template <acceleration_unit U, class Rep = double>
    using Acceleration = quantity<U, Rep>;

    template <force_unit U, class Rep = double>
    using Force = quantity<U, Rep>;

    template <energy_unit U, class Rep = double>
    using Energy = quantity<U, Rep>;

}   // namespace mstd

#endif   // __MSTD_UNITS_QUANTITY_IMPL_HPP__