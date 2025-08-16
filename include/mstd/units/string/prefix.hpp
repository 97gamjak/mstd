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

#ifndef __MSTD_UNITS_STRING_PREFIX_HPP__
#define __MSTD_UNITS_STRING_PREFIX_HPP__

#include <ratio>
#include <string_view>

namespace mstd
{
    namespace units::strings
    {

        template <typename Ratio>
        constexpr std::string_view get_prefix()
        {
            return "###";
        }
        template <>
        constexpr std::string_view get_prefix<std::ratio<1>>()
        {
            return "";
        }
        template <>
        constexpr std::string_view get_prefix<std::femto>()
        {
            return "f";
        }
        template <>
        constexpr std::string_view get_prefix<std::pico>()
        {
            return "p";
        }
        template <>
        constexpr std::string_view get_prefix<std::nano>()
        {
            return "n";
        }
        template <>
        constexpr std::string_view get_prefix<std::micro>()
        {
            return "u";
        }
        template <>
        constexpr std::string_view get_prefix<std::milli>()
        {
            return "m";
        }
        template <>
        constexpr std::string_view get_prefix<std::centi>()
        {
            return "c";
        }
        template <>
        constexpr std::string_view get_prefix<std::deci>()
        {
            return "d";
        }
        template <>
        constexpr std::string_view get_prefix<std::kilo>()
        {
            return "k";
        }
        template <>
        constexpr std::string_view get_prefix<std::mega>()
        {
            return "M";
        }
        template <>
        constexpr std::string_view get_prefix<std::giga>()
        {
            return "G";
        }
        template <>
        constexpr std::string_view get_prefix<std::tera>()
        {
            return "T";
        }

    }   // namespace units::strings

}   // namespace mstd

#endif   // __MSTD_UNITS_STRING_PREFIX_HPP__