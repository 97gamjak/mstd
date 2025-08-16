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

#ifndef __MSTD_UNITS_STRING_BASE_STRING_HPP__
#define __MSTD_UNITS_STRING_BASE_STRING_HPP__

#include <string_view>

#include "../base.hpp"

namespace mstd
{
    namespace units::strings
    {

        template <BaseUnitEnum id>
        inline constexpr std::string_view get_base_unit_string()
        {
            return "";
        }

        template <>
        inline constexpr std::string_view get_base_unit_string<
            BaseUnitEnum::Length>()
        {
            return "m";
        }

        template <>
        inline constexpr std::string_view get_base_unit_string<
            BaseUnitEnum::Mass>()
        {
            return "g";
        }

        template <>
        inline constexpr std::string_view get_base_unit_string<
            BaseUnitEnum::Time>()
        {
            return "s";
        }

        template <>
        inline constexpr std::string_view get_base_unit_string<
            BaseUnitEnum::Temperature>()
        {
            return "K";
        }

        template <>
        inline constexpr std::string_view get_base_unit_string<
            BaseUnitEnum::AmountOfSubstance>()
        {
            return "mol";
        }

        template <>
        inline constexpr std::string_view get_base_unit_string<
            BaseUnitEnum::Current>()
        {
            return "A";
        }

        template <>
        inline constexpr std::string_view get_base_unit_string<
            BaseUnitEnum::LuminousIntensity>()
        {
            return "Cd";
        }

    }   // namespace units::strings

}   // namespace mstd

#endif   // __MSTD_UNITS_STRING_BASE_STRING_HPP__