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

#ifndef __MSTD_RATIO_TRAITS_HPP__
#define __MSTD_RATIO_TRAITS_HPP__

/**
 * @file ratio_traits.hpp
 * @brief Concepts for std::ratio types.
 *
 * Provides concepts to check if a type conforms to the expected interface
 * of std::ratio types.
 */

namespace mstd
{

    /**
     * @brief Concept for std::ratio types.
     *
     * @details A ratio type must provide two static members `num` and `den`.
     */
    template <typename T>
    concept RatioType = requires {
        T::num;
        T::den;
    };

    template <typename T>
    constexpr bool is_ratio_v = RatioType<T>;

}   // namespace mstd

#endif   // __MSTD_RATIO_TRAITS_HPP__