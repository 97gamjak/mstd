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

#ifndef __MSTD__PACK__RATIONAL_PACK_HPP__
#define __MSTD__PACK__RATIONAL_PACK_HPP__

#include <array>
#include <cstddef>
#include <ratio>
#include <tuple>

#include "mstd/math.hpp"
#include "mstd/type_traits.hpp"

/**
 * @file ratio_pack.hpp
 * @brief Public ratio_pack type and high-level aliases.
 *
 * Exposes `RationalPack`, a fixed-size compile-time list of `std::ratio` types,
 * plus helpers to multiply/divide element-wise, raise all entries to a power,
 * and create common pack shapes.
 */

namespace mstd
{
    /*********************
     *                   *
     * Ratio pack type   *
     *                   *
     *********************/

    /**
     * @brief A compile-time list of rational powers.
     *
     * @tparam Rs The ratios to include in the RationalPack.
     */
    template <typename... Rs>
    requires((is_convertible_to_rational_power_v<Rs>) || ...)
    struct RationalPack
    {
        // store actual ratio values as long double so we can index them
        static constexpr std::array<long double, sizeof...(Rs)> vals{Rs::value(
        )...};

        /** Number of ratios stored. */
        static constexpr size_t size = sizeof...(Rs);

        /**
         * @brief Ratio type at a given index.
         *
         * @tparam I Index into the pack.
         */
        template <auto I>
        using type_at = typename std::
            tuple_element<static_cast<size_t>(I), std::tuple<Rs...>>::type;

        /**
         * @brief Get the real ratio value at the specified index
         *
         * @tparam I Index into the pack.
         * @return constexpr long double Value of `type_at<I>` as long double.
         */
        template <auto I>
        static constexpr long double get()
        {
            return vals[static_cast<size_t>(I)];
        }
    };

}   // namespace mstd

#endif   // __MSTD__PACK__RATIONAL_PACK_HPP__
