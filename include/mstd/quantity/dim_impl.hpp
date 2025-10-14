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
#include "dim_operations.hpp"

namespace mstd
{
    /********************************************
     *                                          *
     * some pack helpers for SI and Extra units *
     *                                          *
     ********************************************/

    using si_one = make_default_integer_pack_t<SIDimIdMeta::size>;

    using si_length = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Length)>;

    using si_inverse_length = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Length),
        -1>;

    using si_mass = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Mass)>;

    using si_inverse_mass = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Mass),
        -1>;

    using si_time = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Time)>;

    using si_inverse_time = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Time),
        -1>;

    using si_current = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Current)>;

    using si_inverse_current = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Current),
        -1>;

    using si_temperature = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Temperature)>;

    using si_inverse_temperature = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Temperature),
        -1>;

    using si_amount = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Amount)>;

    using si_inverse_amount = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Amount),
        -1>;

    using si_luminous = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Luminous)>;

    using si_inverse_luminous = make_single_integer_pack_t<
        SIDimIdMeta::size,
        SIDimIdMeta::to_underlying(SIDimId::Luminous),
        -1>;

    using ex_one = make_default_integer_pack_t<ExtraDimIdMeta::size>;

    using ex_angle = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Angle)>;

    using ex_inverse_angle = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Angle),
        -1>;

    using ex_currency = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Currency)>;

    using ex_inverse_currency = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Currency),
        -1>;

    using ex_info = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Info)>;

    using ex_inverse_info = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Info),
        -1>;

    using ex_count = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Count)>;

    using ex_inverse_count = make_single_integer_pack_t<
        ExtraDimIdMeta::size,
        ExtraDimIdMeta::to_underlying(ExtraDimId::Count),
        -1>;

    /**********************
     *                    *
     * Base SI dimensions *
     *                    *
     **********************/

    using dim_one      = Dim<si_one, ex_one>;
    using dim_scalar   = dim_one;
    using dim_length   = Dim<si_length, ex_one>;
    using dim_mass     = Dim<si_mass, ex_one>;
    using dim_time     = Dim<si_time, ex_one>;
    using dim_current  = Dim<si_current, ex_one>;
    using dim_temp     = Dim<si_temperature, ex_one>;
    using dim_amount   = Dim<si_amount, ex_one>;
    using dim_luminous = Dim<si_luminous, ex_one>;

    using dim_inv_length = Dim<si_length, ex_one>;
    using dim_inv_mass   = Dim<si_inverse_mass, ex_one>;
    using dim_inv_time   = Dim<si_inverse_time, ex_one>;
    using dim_inv_curr   = Dim<si_inverse_current, ex_one>;
    using dim_inv_temp   = Dim<si_inverse_temperature, ex_one>;
    using dim_inv_amount = Dim<si_inverse_amount, ex_one>;
    using dim_inv_lum    = Dim<si_inverse_luminous, ex_one>;

    /********************
     *                  *
     * Other base units *
     *                  *
     ********************/

    using dim_angle    = Dim<si_one, ex_angle>;
    using dim_currency = Dim<si_one, ex_currency>;
    using dim_info     = Dim<si_one, ex_info>;

    using dim_inv_angle    = Dim<si_one, ex_inverse_angle>;
    using dim_inv_currency = Dim<si_one, ex_inverse_currency>;
    using dim_inv_info     = Dim<si_one, ex_inverse_info>;

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

    using dim_inv_area         = dim_mul_t<dim_inv_length, dim_inv_length>;
    using dim_inv_volume       = dim_mul_t<dim_inv_area, dim_inv_length>;
    using dim_inv_density      = dim_div_t<dim_inv_mass, dim_inv_volume>;
    using dim_inv_velocity     = dim_div_t<dim_inv_length, dim_inv_time>;
    using dim_inv_acceleration = dim_div_t<dim_inv_velocity, dim_inv_time>;
    using dim_inv_force        = dim_mul_t<dim_inv_mass, dim_inv_acceleration>;
    using dim_inv_energy       = dim_mul_t<dim_inv_force, dim_inv_length>;
    using dim_inv_power        = dim_div_t<dim_inv_energy, dim_inv_time>;

}   // namespace mstd

#endif   // __MSTD_DIMENSION_IMPL_HPP__