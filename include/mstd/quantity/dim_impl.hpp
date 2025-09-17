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

#ifndef __MSTD_DIMENSION_IMPL_HPP__
#define __MSTD_DIMENSION_IMPL_HPP__

#include "dim.hpp"

namespace mstd::units
{
    /********************************************
     *                                          *
     * some pack helpers for SI and Extra units *
     *                                          *
     ********************************************/

    using si_one = pack::make_default_integer_pack_t<SIDimIdMeta::size>;

    using si_length = pack::make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Length)>;

    using si_mass = pack::make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Mass)>;

    using si_time = pack::make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Time)>;

    using si_current = pack::make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Current)>;

    using si_temperature = pack::make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Temperature)>;

    using si_amount = pack::make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Amount)>;

    using si_luminous = pack::make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Luminous)>;

    using ex_one = pack::make_default_integer_pack_t<ExtraDimIdMeta::size>;

    using ex_angle = pack::make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Angle)>;

    using ex_currency = pack::make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Currency)>;

    using ex_info = pack::make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Info)>;

    using ex_count = pack::make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Count)>;

    /**********************
     *                    *
     * Base SI dimensions *
     *                    *
     **********************/

    using dim_one      = dim<si_one, ex_one>;
    using dim_scalar   = dim_one;
    using dim_length   = dim<si_length, ex_one>;
    using dim_mass     = dim<si_mass, ex_one>;
    using dim_time     = dim<si_time, ex_one>;
    using dim_current  = dim<si_current, ex_one>;
    using dim_temp     = dim<si_temperature, ex_one>;
    using dim_amount   = dim<si_amount, ex_one>;
    using dim_luminous = dim<si_luminous, ex_one>;

    /********************
     *                  *
     * Other base units *
     *                  *
     ********************/

    using dim_angle    = dim<si_one, ex_angle>;
    using dim_currency = dim<si_one, ex_currency>;
    using dim_info     = dim<si_one, ex_info>;

    /************************
     *                      *
     * Common derived units *
     *                      *
     ************************/

    using dim_area         = dim_mul_t<dim_length, dim_length>;
    using dim_volume       = dim_mul_t<dim_area, dim_length>;
    using dim_density      = dim_div_t<dim_mass, dim_volume>;
    using dim_velocity     = dim_div_t<dim_length, dim_time>;
    using dim_acceleration = dim_div_t<dim_velocity, dim_time>;
    using dim_force        = dim_mul_t<dim_mass, dim_acceleration>;
    using dim_energy       = dim_mul_t<dim_force, dim_length>;
    using dim_power        = dim_div_t<dim_energy, dim_time>;

}   // namespace mstd::units

#endif   // __MSTD_DIMENSION_IMPL_HPP__