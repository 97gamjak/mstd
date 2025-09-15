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

#include "dimension_impl.hpp"
#include "unit.hpp"
#include "unit_operations.hpp"

namespace mstd::units
{
    /*****************
     *               *
     * Unit concepts *
     *               *
     *****************/

    // clang-format off
    template <class U>
    concept length_unit = std::is_same_v<typename unit_traits<U>::dim, dim_length>;
    template <class U>
    concept mass_unit = std::is_same_v<typename unit_traits<U>::dim, dim_mass>;
    template <class U>
    concept time_unit = std::is_same_v<typename unit_traits<U>::dim, dim_time>;
    template <class U>
    concept current_unit = std::is_same_v<typename unit_traits<U>::dim, dim_current>;
    template <class U>
    concept temperature_unit = std::is_same_v<typename unit_traits<U>::dim, dim_temp>;
    template <class U>
    concept amount_unit = std::is_same_v<typename unit_traits<U>::dim, dim_amount>;
    template <class U>
    concept luminous_unit = std::is_same_v<typename unit_traits<U>::dim, dim_luminous>;

    template <class U>
    concept angle_unit = std::is_same_v<typename unit_traits<U>::dim, dim_angle>;
    template <class U>
    concept currency_unit = std::is_same_v<typename unit_traits<U>::dim, dim_currency>;
    template <class U>
    concept info_unit = std::is_same_v<typename unit_traits<U>::dim, dim_info>;
    template <class U>
    concept scalar_unit = std::is_same_v<typename unit_traits<U>::dim, dim_scalar>;
    template <class U>
    concept dimensionless_unit = scalar_unit<U> && std::is_same_v<typename U::ratio, std::ratio<1>>;

    template <class U>
    concept area_unit = std::is_same_v<typename unit_traits<U>::dim, dim_area>;
    template <class U>
    concept volume_unit = std::is_same_v<typename unit_traits<U>::dim, dim_volume>;
    template <class U>
    concept density_unit = std::is_same_v<typename unit_traits<U>::dim, dim_density>;
    template <class U>
    concept velocity_unit = std::is_same_v<typename unit_traits<
    U>::dim, dim_velocity>;
    template <class U>
    concept acceleration_unit = std::is_same_v<typename unit_traits<U>::dim, dim_acceleration>;
    template <class U>
    concept force_unit = std::is_same_v<typename unit_traits<U>::dim, dim_force>;
    template <class U>
    concept energy_unit = std::is_same_v<typename unit_traits<U>::dim, dim_energy>;
    template <class U>
    concept power_unit = std::is_same_v<typename unit_traits<U>::dim, dim_power>;
    // clang-format on

    /***********************
     *                     *
     * Common unit aliases *
     *                     *
     ***********************/

    namespace literals
    {
        static constexpr int         __M_IN_ANG_EXP__ = 10;
        static constexpr long double __AMU_TO_KG__    = 1.66053906660e-27L;

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

        using unitless = unit<dim_scalar, std::ratio<1>>;

        using km  = unit<dim_length, std::kilo>;
        using m   = unit<dim_length, std::ratio<1>>;
        using dm  = unit<dim_length, std::deci>;
        using cm  = unit<dim_length, std::centi>;
        using mm  = unit<dim_length, std::milli>;
        using um  = unit<dim_length, std::micro>;
        using nm  = unit<dim_length, std::nano>;
        using pm  = unit<dim_length, std::pico>;
        using fm  = unit<dim_length, std::femto>;
        using Ang = unit<dim_length, std::ratio<1, __M_IN_ANG_EXP__>>;

        using t   = unit<dim_mass, std::kilo>;
        using kg  = unit<dim_mass, std::ratio<1>>;
        using g   = unit<dim_mass, std::milli>;
        using mg  = unit<dim_mass, std::micro>;
        using ug  = unit<dim_mass, std::nano>;
        using ng  = unit<dim_mass, std::pico>;
        using pg  = unit<dim_mass, std::femto>;
        using amu = real_unit<dim_mass, __AMU_TO_KG__, std::ratio<1>>;

        using y   = unit<dim_time, std::ratio<__Y_TO_S__>>;
        using d   = unit<dim_time, std::ratio<__D_TO_S__>>;
        using h   = unit<dim_time, std::ratio<__H_TO_S__>>;
        using min = unit<dim_time, std::ratio<__MIN_TO_S__>>;
        using s   = unit<dim_time, std::ratio<1>>;
        using ms  = unit<dim_time, std::milli>;
        using us  = unit<dim_time, std::micro>;
        using ns  = unit<dim_time, std::nano>;
        using ps  = unit<dim_time, std::pico>;
        using fs  = unit<dim_time, std::femto>;

        using A  = unit<dim_current, std::ratio<1>>;
        using mA = unit<dim_current, std::milli>;
        using uA = unit<dim_current, std::micro>;
        using nA = unit<dim_current, std::nano>;
        using pA = unit<dim_current, std::pico>;
        using fA = unit<dim_current, std::femto>;

        using K  = unit<dim_temp, std::ratio<1>>;
        using mK = unit<dim_temp, std::milli>;
        using uK = unit<dim_temp, std::micro>;
        using nK = unit<dim_temp, std::nano>;
        using pK = unit<dim_temp, std::pico>;
        using fK = unit<dim_temp, std::femto>;

        using mol  = unit<dim_amount, std::ratio<1>>;
        using mmol = unit<dim_amount, std::milli>;
        using umol = unit<dim_amount, std::micro>;
        using nmol = unit<dim_amount, std::nano>;
        using pmol = unit<dim_amount, std::pico>;

        using cd  = unit<dim_luminous, std::ratio<1>>;
        using mcd = unit<dim_luminous, std::milli>;
        using ucd = unit<dim_luminous, std::micro>;
        using ncd = unit<dim_luminous, std::nano>;
        using pcd = unit<dim_luminous, std::pico>;
        using fcd = unit<dim_luminous, std::femto>;

        using rad = unit<dim_angle, std::ratio<1>>;
        using deg = real_unit<dim_angle, __RAD_TO_DEG__, std::ratio<1>>;

        using mps = unit_div<m, s>;
        using kmh = unit_div<km, h>;
        using c =
            real_unit<dim_velocity, __SPEED_OF_LIGHT_TO_MPS__, std::ratio<1>>;

        using mps2 = unit_div<mps, s>;
        using test = unit_mul<m, s>;

        using N  = unit_mul<kg, mps2>;
        using kN = unit<N::dim, std::kilo>;
        using MN = unit<N::dim, std::mega>;

        using J    = unit_mul<N, m>;
        using kJ   = unit<J::dim, std::kilo>;
        using MJ   = unit<J::dim, std::mega>;
        using GJ   = unit<J::dim, std::giga>;
        using cal  = real_unit<J::dim, __CAL_TO_J__, std::ratio<1>>;
        using kcal = real_unit<cal::dim, cal::factor, std::ratio<1>, std::kilo>;
        using _    = typename kcal::dim;

        using W  = unit_div<J, s>;
        using kW = unit<W::dim, std::kilo>;
        using MW = unit<W::dim, std::mega>;
        using GW = unit<W::dim, std::giga>;

    }   // namespace literals

}   // namespace mstd::units

#endif   // __MSTD_UNIT_IMPL_HPP__