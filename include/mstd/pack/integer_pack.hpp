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

#ifndef __MSTD_INTEGER_PACK_HPP__
#define __MSTD_INTEGER_PACK_HPP__

#include <array>
#include <cstddef>

#include "integer_pack_details.hpp"
#include "mstd/functional.hpp"

/**
 * @file integer_pack.hpp
 * @brief Public integer_pack type and high-level aliases.
 *
 * Exposes `integer_pack`, a fixed-size compile-time list of ints, plus common
 * pack operations (add, sub, scale) and simple factory aliases.
 */

namespace mstd::pack
{
    /***********************
     *                     *
     * Integer pack type   *
     *                     *
     ***********************/
    /**
     * @brief A compile-time list of integers.
     *
     * @tparam Es The integers to include in the integer_pack.
     */
    template <int... Es>
    struct integer_pack
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

            return -1;
        }
    };

    /*************************
     *                       *
     * Convenience aliases   *
     *                       *
     *************************/
    /** Add two packs element-wise. */
    template <class A, class B>
    using pack_add_t = details::pack_zip_t<A, B, functional::add_fn>;
    /** Subtract two packs element-wise. */
    template <class A, class B>
    using pack_sub_t = details::pack_zip_t<A, B, functional::sub_fn>;
    /** Scale a pack by a compile-time integer K. */
    template <class P, int K>
    using pack_scale_t = details::pack_map_t<P, functional::scale_fn<K>>;

    /*********************
     *                   *
     * Factory aliases   *
     *                   *
     *********************/
    /** Create an `integer_pack` of size N filled with zeros. */
    template <std::size_t N>
    using make_default_integer_pack_t =
        typename details::make_default_integer_pack<N>::type;

    /** Create an `integer_pack` of size N with 0s except index Ix set to I. */
    template <std::size_t N, std::size_t Ix, int I = 1>
    using make_single_integer_pack_t =
        typename details::make_single_integer_pack<N, Ix, I>::type;

}   // namespace mstd::pack

#endif   // __MSTD_INTEGER_PACK_HPP__
