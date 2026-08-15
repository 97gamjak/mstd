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

#ifndef __MSTD__TYPE_TRAITS__STRING_HPP__
#define __MSTD__TYPE_TRAITS__STRING_HPP__

#include <concepts>
#include <string>

namespace mstd
{
    /**
     * @brief Concept checking whether Tag::toString(const T&) is a valid
     * expression that returns something convertible to std::string.
     *
     * @tparam Tag the tag type expected to provide a static toString member
     * @tparam T the value type passed to Tag::toString
     */
    template <typename Tag, typename T>
    concept HasToString = requires(const T &value) {
        { Tag::toString(value) } -> std::convertible_to<std::string>;
    };
}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__STRING_HPP__
