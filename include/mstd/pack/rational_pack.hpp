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
        /** Number of ratios stored. */
        static constexpr size_t size = sizeof...(Rs);

        static_assert(size > 0, "RationalPack must contain at least one ratio");

        // store actual ratio values as long double so we can index them
        static constexpr std::array<long double, sizeof...(Rs)> vals{Rs::value(
        )...};

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

        static constexpr size_t num_non_zero()
        {
            size_t count = 0;
            for (size_t i = 0; i < size; ++i)
                if (vals[i] != 0.0L)
                    ++count;

            return count;
        }

        /**
         * @brief Index of the first non-zero entry.
         *
         * Returns `static_cast<size_t>(-1)` if all entries are zero.
         */
        static constexpr size_t first_non_zero_index()
        {
            if constexpr (num_non_zero() == 0)
                return -1;

            for (size_t i = 0; i < size; ++i)
                if (vals[i] != 0.0L)
                    return i;

            return -1;
        }

        static constexpr long double value()
        {
            long double r = 1.0L;
            for (size_t i = 0; i < size; ++i)
                r *= vals[i];
            return r;
        }
    };

    template <typename Pack>
    struct pack_utils
    {
        static constexpr std::size_t count_non_zero()
        {
            return count_non_zero_impl(std::make_index_sequence<Pack::size>{});
        }

       private:
        template <std::size_t... Is>
        static consteval std::size_t count_non_zero_impl(
            std::index_sequence<Is...>
        )
        {
            return (
                (!(Pack::template type_at<Is>::is_zero) ? 1u : 0u) + ... + 0u
            );
        }
    };

    template <typename A>
    requires RationalPackType<A> || RationalPowerPackType<A>
    struct ratio_v_impl<A>
    {
        static constexpr long double value = A::value();
    };

}   // namespace mstd

#endif   // __MSTD__PACK__RATIONAL_PACK_HPP__