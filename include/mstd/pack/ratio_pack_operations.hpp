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

#ifndef __MSTD_RATIO_PACK_OPERATIONS_HPP__
#define __MSTD_RATIO_PACK_OPERATIONS_HPP__

#include "ratio_pack.hpp"
#include "ratio_pack_details.hpp"

namespace mstd
{
    /**
     * @brief Get the value of a RatioPack as long double
     *
     * @tparam Pack The RatioPack type
     * @return constexpr long double The product of all ratios in the pack as
     * long double
     */
    template <class Pack>
    inline constexpr long double ratio_pack_v =
        details::ratio_pack_value_impl<Pack>(
            std::make_index_sequence<Pack::size>{}
        );

    /*************************
     *                       *
     * Convenience aliases   *
     *                       *
     *************************/

    /** Element-wise multiply two ratio packs. */
    template <RatioPackType A, RatioPackType B>
    using ratio_pack_mul_t =
        details::ratio_pack_zip_t<A, B, std::ratio_multiply>;

    /** Element-wise divide two ratio packs. */
    template <RatioPackType A, RatioPackType B>
    using ratio_pack_div_t = details::ratio_pack_zip_t<A, B, std::ratio_divide>;

    /** Raise each ratio in a pack to integer power K. */
    template <RatioPackType Pack, int K>
    using ratio_pack_pow_t = decltype(details::ratio_pack_pow_impl<Pack, K>(
        std::make_index_sequence<Pack::size>{}
    ));

    /*********************
     *                   *
     * Factory aliases   *
     *                   *
     *********************/

    /** Create a RatioPack of size N filled with `std::ratio<1>`. */
    template <std::size_t N>
    using make_default_ratio_pack_t =
        typename details::make_default_ratio_pack<N>::type;

    /** Create a RatioPack of size N with a single ratio R at index Idx. */
    template <StdRatio R, size_t Idx, size_t N>
    using make_ratio_pack_single_t =
        decltype(details::make_ratio_pack_at_impl_R<R, Idx>(
            std::make_index_sequence<N>{}
        ));

}   // namespace mstd

#endif   // __MSTD_RATIO_PACK_OPERATIONS_HPP__