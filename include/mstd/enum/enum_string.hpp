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

#ifndef __MSTD__ENUM__ENUM_STRING_HPP__
#define __MSTD__ENUM__ENUM_STRING_HPP__

#include <array>
#include <string_view>
#include <utility>

namespace mstd
{
    /**
     * @brief Provides compile-time mappings between enum values and their
     * string representations.
     *
     * @tparam T The enum type for which the mappings are provided.
     */
    template <typename T>
    struct EnumFromString
    {
        static constexpr std::array<std::pair<std::string_view, T>, 0>
            value{};
    };

    /**
     * @brief Provides compile-time mappings between enum values and their
     * string representations.
     *
     * @tparam T The enum type for which the mappings are provided.
     */
    template <typename T>
    struct EnumToString
    {
        static constexpr std::array<std::pair<T, std::string_view>, 0>
            value{};
    };
}   // namespace mstd

#endif   // __MSTD__ENUM__ENUM_STRING_HPP__
