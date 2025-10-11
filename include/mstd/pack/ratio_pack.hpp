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

#ifndef __MSTD_RATIO_PACK_HPP__
#define __MSTD_RATIO_PACK_HPP__

#include <array>
#include <cstddef>
#include <ratio>
#include <tuple>

#include "mstd/ratio.hpp"
#include "ratio_pack_details.hpp"

/**
 * @file ratio_pack.hpp
 * @brief Public ratio_pack type and high-level aliases.
 *
 * Exposes `ratio_pack`, a fixed-size compile-time list of `std::ratio` types,
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
     * @brief A compile-time list of ratios.
     *
     * @tparam Rs The ratios to include in the ratio_pack.
     */
    template <class... Rs>
    struct ratio_pack
    {
        // store actual ratio values as long double so we can index them
        static constexpr std::array<long double, sizeof...(Rs)> vals{
            static_cast<long double>(Rs::num) / Rs::den...
        };

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

    /*************************
     *                       *
     * Convenience aliases   *
     *                       *
     *************************/
    /** Element-wise multiply two ratio packs. */
    template <details::RatioPack A, details::RatioPack B>
    using ratio_pack_mul_t =
        details::ratio_pack_zip_t<A, B, std::ratio_multiply>;

    /** Element-wise divide two ratio packs. */
    template <details::RatioPack A, details::RatioPack B>
    using ratio_pack_div_t = details::ratio_pack_zip_t<A, B, std::ratio_divide>;

    /** Raise each ratio in a pack to integer power K. */
    template <details::RatioPack Pack, int K>
    using ratio_pack_pow_t = decltype(details::ratio_pack_pow_impl<Pack, K>(
        std::make_index_sequence<Pack::size>{}
    ));

    /*********************
     *                   *
     * Factory aliases   *
     *                   *
     *********************/
    /** Create a ratio_pack of size N filled with `std::ratio<1>`. */
    template <std::size_t N>
    using make_default_ratio_pack_t =
        typename details::make_default_ratio_pack<N>::type;

    /** Create a ratio_pack of size N with a single ratio R at index Idx. */
    template <StdRatio R, size_t Idx, size_t N>
    using make_ratio_pack_single_t =
        decltype(details::make_ratio_pack_at_impl_R<R, Idx>(
            std::make_index_sequence<N>{}
        ));

}   // namespace mstd

#endif   // __MSTD_RATIO_PACK_HPP__
