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

#ifndef __MSTD_UNIT_IMPL_HPP__
#define __MSTD_UNIT_IMPL_HPP__

#include <numbers>

#include "dim_impl.hpp"
#include "dim_ratio_impl.hpp"
#include "unit.hpp"
#include "unit_operations.hpp"

namespace mstd
{
    /*****************
     *               *
     * Unit concepts *
     *               *
     *****************/

    // clang-format off
    template <class U>
    concept length_unit = has_dim_v<U, dim_length>;
    template <class U>
    concept mass_unit = has_dim_v<U, dim_mass>;
    template <class U>
    concept time_unit = has_dim_v<U, dim_time>;
    template <class U>
    concept current_unit = has_dim_v<U, dim_current>;
    template <class U>
    concept temperature_unit = has_dim_v<U, dim_temp>;
    template <class U>
    concept amount_unit = has_dim_v<U, dim_amount>;
    template <class U>
    concept luminous_unit = has_dim_v<U, dim_luminous>;

    template <class U>
    concept angle_unit = has_dim_v<U, dim_angle>;
    template <class U>
    concept currency_unit = has_dim_v<U, dim_currency>;
    template <class U>
    concept info_unit = has_dim_v<U, dim_info>;
    template <class U>
    concept scalar_unit = has_dim_v<U, dim_scalar>;
    template <class U>
    concept dimensionless_unit = scalar_unit<U> && std::is_same_v<typename U::ratio, std::ratio<1>>;

    template <class U>
    concept area_unit = has_dim_v<U, dim_area>;
    template <class U>
    concept volume_unit = has_dim_v<U, dim_volume>;
    template <class U>
    concept density_unit = has_dim_v<U, dim_density>;
    template <class U>
    concept velocity_unit = has_dim_v<U, dim_velocity>;
    template <class U>
    concept acceleration_unit = has_dim_v<U, dim_acceleration>;
    template <class U>
    concept force_unit = has_dim_v<U, dim_force>;
    template <class U>
    concept energy_unit = has_dim_v<U, dim_energy>;
    template <class U>
    concept power_unit = has_dim_v<U, dim_power>;
    // clang-format on

    /***********************
     *                     *
     * Common unit aliases *
     *                     *
     ***********************/

    namespace literals
    {
        static constexpr long double   __AMU_TO_KG__    = 1.66053906660e-27L;
        static constexpr long long int __M_IN_ANG_EXP__ = 1E10;

        using __ANG_IN_M_RATIO__ = std::ratio<1, __M_IN_ANG_EXP__>;

        static constexpr int __MIN_TO_S__ = 60;
        static constexpr int __H_TO_MIN__ = 60;
        static constexpr int __H_TO_S__   = __MIN_TO_S__ * __H_TO_MIN__;
        static constexpr int __D_TO_H__   = 24;
        static constexpr int __D_TO_S__   = __H_TO_S__ * __D_TO_H__;
        static constexpr int __Y_TO_D__   = 365;
        static constexpr int __Y_TO_S__   = __D_TO_S__ * __Y_TO_D__;

        static constexpr long double __RAD_TO_DEG__ =
            180.0L / std::numbers::pi_v<long double>;

        static constexpr long double __SPEED_OF_LIGHT_TO_MPS__ = 299792458.0L;

        static constexpr long double __CAL_TO_J__ = 4.184L;

        using unitless = Unit<dim_scalar, length_dim_ratio<>>;

        using km  = Unit<dim_length, length_dim_ratio<std::kilo>>;
        using m   = Unit<dim_length, length_dim_ratio<>>;
        using dm  = Unit<dim_length, length_dim_ratio<std::deci>>;
        using cm  = Unit<dim_length, length_dim_ratio<std::centi>>;
        using mm  = Unit<dim_length, length_dim_ratio<std::milli>>;
        using um  = Unit<dim_length, length_dim_ratio<std::micro>>;
        using nm  = Unit<dim_length, length_dim_ratio<std::nano>>;
        using pm  = Unit<dim_length, length_dim_ratio<std::pico>>;
        using fm  = Unit<dim_length, length_dim_ratio<std::femto>>;
        using Ang = Unit<dim_length, length_dim_ratio<__ANG_IN_M_RATIO__>>;

        using t   = Unit<dim_mass, mass_dim_ratio<std::kilo>>;
        using kg  = Unit<dim_mass, mass_dim_ratio<std::ratio<1>>>;
        using g   = Unit<dim_mass, mass_dim_ratio<std::milli>>;
        using mg  = Unit<dim_mass, mass_dim_ratio<std::micro>>;
        using ug  = Unit<dim_mass, mass_dim_ratio<std::nano>>;
        using ng  = Unit<dim_mass, mass_dim_ratio<std::pico>>;
        using pg  = Unit<dim_mass, mass_dim_ratio<std::femto>>;
        using amu = scaled_unit_t<kg, __AMU_TO_KG__>;

        using y   = Unit<dim_time, time_dim_ratio<std::ratio<__Y_TO_S__>>>;
        using d   = Unit<dim_time, time_dim_ratio<std::ratio<__D_TO_S__>>>;
        using h   = Unit<dim_time, time_dim_ratio<std::ratio<__H_TO_S__>>>;
        using min = Unit<dim_time, time_dim_ratio<std::ratio<__MIN_TO_S__>>>;
        using s   = Unit<dim_time, time_dim_ratio<>>;
        using ms  = Unit<dim_time, time_dim_ratio<std::milli>>;
        using us  = Unit<dim_time, time_dim_ratio<std::micro>>;
        using ns  = Unit<dim_time, time_dim_ratio<std::nano>>;
        using ps  = Unit<dim_time, time_dim_ratio<std::pico>>;
        using fs  = Unit<dim_time, time_dim_ratio<std::femto>>;

        using A  = Unit<dim_current, current_dim_ratio<>>;
        using mA = Unit<dim_current, current_dim_ratio<std::milli>>;
        using uA = Unit<dim_current, current_dim_ratio<std::micro>>;
        using nA = Unit<dim_current, current_dim_ratio<std::nano>>;
        using pA = Unit<dim_current, current_dim_ratio<std::pico>>;
        using fA = Unit<dim_current, current_dim_ratio<std::femto>>;

        using K  = Unit<dim_temp, temp_dim_ratio<>>;
        using mK = Unit<dim_temp, temp_dim_ratio<std::milli>>;
        using uK = Unit<dim_temp, temp_dim_ratio<std::micro>>;
        using nK = Unit<dim_temp, temp_dim_ratio<std::nano>>;
        using pK = Unit<dim_temp, temp_dim_ratio<std::pico>>;
        using fK = Unit<dim_temp, temp_dim_ratio<std::femto>>;

        using mol  = Unit<dim_amount, amount_dim_ratio<>>;
        using mmol = Unit<dim_amount, amount_dim_ratio<std::milli>>;
        using umol = Unit<dim_amount, amount_dim_ratio<std::micro>>;
        using nmol = Unit<dim_amount, amount_dim_ratio<std::nano>>;
        using pmol = Unit<dim_amount, amount_dim_ratio<std::pico>>;

        using cd  = Unit<dim_luminous, luminous_dim_ratio<>>;
        using mcd = Unit<dim_luminous, luminous_dim_ratio<std::milli>>;
        using ucd = Unit<dim_luminous, luminous_dim_ratio<std::micro>>;
        using ncd = Unit<dim_luminous, luminous_dim_ratio<std::nano>>;
        using pcd = Unit<dim_luminous, luminous_dim_ratio<std::pico>>;
        using fcd = Unit<dim_luminous, luminous_dim_ratio<std::femto>>;

        using rad = Unit<dim_angle, angle_dim_ratio<>>;
        using deg = scaled_unit_t<rad, __RAD_TO_DEG__>;

        using m_per_s   = unit_div_t<m, s>;
        using km_per_h  = unit_div_t<km, h>;
        using Ang_per_s = unit_div_t<Ang, s>;
        using c         = scaled_unit_t<m_per_s, __SPEED_OF_LIGHT_TO_MPS__>;

        using m_per_s2 = unit_div_t<m_per_s, s>;

        using N  = unit_mul_t<kg, m_per_s2>;
        using kN = Unit<N::dim, DimRatio<>, std::kilo>;
        using MN = Unit<N::dim, DimRatio<>, std::mega>;

        using J    = unit_mul_t<N, m>;
        using kJ   = Unit<J::dim, DimRatio<>, std::kilo>;
        using MJ   = Unit<J::dim, DimRatio<>, std::mega>;
        using GJ   = Unit<J::dim, DimRatio<>, std::giga>;
        using cal  = scaled_unit_t<J, __CAL_TO_J__>;
        using kcal = scaled_unit_t<kJ, cal::factor_v>;

        using kcal_per_mol         = unit_div_t<kcal, mol>;
        using kcal_per_mol_per_Ang = unit_div_t<kcal, mol, Ang>;

        using W  = unit_div_t<J, s>;
        using kW = Unit<W::dim, DimRatio<>, std::kilo>;
        using MW = Unit<W::dim, DimRatio<>, std::mega>;
        using GW = Unit<W::dim, DimRatio<>, std::giga>;

    }   // namespace literals

}   // namespace mstd

#endif   // __MSTD_UNIT_IMPL_HPP__