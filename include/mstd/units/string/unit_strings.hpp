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

#ifndef __MSTD_UNITS_STRINGS_HPP__
#define __MSTD_UNITS_STRINGS_HPP__

#include <optional>
#include <ratio>
#include <string>

#include "../base.hpp"
#include "../scale_tags.hpp"
#include "base_string.hpp"
#include "prefix.hpp"

namespace mstd
{
    namespace units::strings
    {

        template <typename Dim, typename Ratio, BaseUnitEnum enumValue>
        constexpr void get_special_unit_cases(
            std::optional<std::string>& prefix,
            std::optional<std::string>& base_unit,
            std::optional<std::string>& exponent_str
        )
        {
            constexpr auto id       = static_cast<std::size_t>(enumValue);
            constexpr auto exponent = Dim::exponents[id];

            if (id == Dim::mass_index())
            {
                prefix = get_prefix<std::ratio_multiply<Ratio, std::kilo>>();
                return;
            }

            if (id == Dim::length_index())
            {
                if (std::same_as<Ratio, Ang::ratio>)
                {
                    prefix    = "";
                    base_unit = "Ang";
                }

                if (std::same_as<Ratio, dm::ratio> && exponent == 3)
                {
                    prefix       = "";
                    base_unit    = "l";
                    exponent_str = "";
                }
            }
        }

        template <typename Dim, typename Pack, BaseUnitEnum enumValue>
        constexpr std::string get_sub_unit()
        {
            using Ratio              = typename Pack::ratio;
            constexpr auto base_unit = get_base_unit_string<enumValue>();
            constexpr auto id        = static_cast<std::size_t>(enumValue);
            constexpr auto exponent  = Dim::exponents[id];

            if (exponent == 0)
                return "";   // no contribution to the unit name

            std::optional<std::string> prefix_str;
            std::optional<std::string> base_unit_str = std::string(base_unit);
            std::optional<std::string> exponent_str;

            get_special_unit_cases<Dim, Ratio, enumValue>(
                prefix_str,
                base_unit_str,
                exponent_str
            );

            // mass unit needs to be shifted as kg is the base unit
            if (!prefix_str.has_value())
                prefix_str = get_prefix<Ratio>();

            if (!exponent_str.has_value() && exponent != 1)
                exponent_str = "^" + std::to_string(exponent);
            else if (!exponent_str.has_value())
                exponent_str = "";

            return prefix_str.value() + base_unit_str.value() +
                   exponent_str.value();
        }

        template <typename Dim, typename Pack>
        inline constexpr std::string get_unit_name()
        {
            std::string unit = "";

            using enum BaseUnitEnum;

            unit += get_sub_unit<Dim, typename Pack::L, Length>();
            unit += get_sub_unit<Dim, typename Pack::M, Mass>();
            unit += get_sub_unit<Dim, typename Pack::T, Time>();
            unit += get_sub_unit<Dim, typename Pack::K, Temperature>();
            unit += get_sub_unit<Dim, typename Pack::N, AmountOfSubstance>();
            unit += get_sub_unit<Dim, typename Pack::I, Current>();
            unit += get_sub_unit<Dim, typename Pack::Cd, LuminousIntensity>();

            return unit;
        }

    }   // namespace units::strings

}   // namespace mstd

#endif   // __MSTD_UNITS_STRINGS_HPP__