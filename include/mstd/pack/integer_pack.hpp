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

#ifndef __MSTD__PACK__INTEGER_PACK_HPP__
#define __MSTD__PACK__INTEGER_PACK_HPP__

#include <array>
#include <cstddef>

#include "mstd/functional.hpp"
#include "mstd/type_traits/pack_traits.hpp"

/**
 * @file integer_pack.hpp
 * @brief Public IntegerPack type and high-level aliases.
 *
 * Exposes `IntegerPack`, a fixed-size compile-time list of ints, plus common
 * pack operations (add, sub, scale) and simple factory aliases.
 */

namespace mstd
{
    /***********************
     *                     *
     * Integer pack type   *
     *                     *
     ***********************/

    /**
     * @brief A compile-time list of integers.
     *
     * @tparam Es The integers to include in the IntegerPack.
     */
    template <int... Es>
    struct IntegerPack
    {
        static constexpr std::array<int, sizeof...(Es)> vals{Es...};

        static constexpr size_t size = sizeof...(Es);

        /**
         * @brief Get the value at the specified index.
         *
         * @tparam I The index of the value to retrieve.
         * @return constexpr int The value at the specified index.
         */
        template <auto I>
        static constexpr int get()
        {
            return vals[static_cast<size_t>(I)];
        }

        /**
         * @brief Number of non-zero entries in the pack.
         */
        static constexpr size_t num_vals_set()
        {
            size_t count = 0;
            for (size_t i = 0; i < size; ++i)
                if (vals[i] != 0)
                    ++count;

            return count;
        }

        /**
         * @brief Index of the first non-zero entry.
         *
         * Returns `static_cast<size_t>(-1)` if all entries are zero.
         */
        static constexpr size_t non_zero_index()
        {
            for (size_t i = 0; i < size; ++i)
                if (vals[i] != 0)
                    return i;

            return static_cast<size_t>(-1);
        }
    };

}   // namespace mstd

#endif   // __MSTD__PACK__INTEGER_PACK_HPP__
