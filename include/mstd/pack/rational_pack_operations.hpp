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

#include "mstd/error.hpp"
#include "rational_pack.hpp"
#include "rational_pack_details.hpp"

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
    struct mul_type<A, B>
    {
        using type = details::ratio_pack_zip_t<A, B, mul_type_t>;
    };

    /** Element-wise divide two ratio packs. */
    template <RatioPackType A, RatioPackType B>
    struct div_type<A, B>
    {
        using type = details::ratio_pack_zip_t<A, B, div_type_t>;
    };

    /** Raise each ratio in a pack to integer powers from an IntegerPack. */
    template <RatioPackType Pack, IntegerPackType IntPack>   // TODO: check this
    struct pow_type<Pack, IntPack>
    {
        using type = details::ratio_pack_pow_impl<Pack, IntPack>::type;
    };

    /** Raise each ratio in a pack to integer power K. */
    template <RatioPackType Pack, int K>   // TODO: check this
    using ratio_pack_pow_k_t = details::ratio_pack_pow_k_impl<Pack, K>::type;

    /*********************
     *                   *
     * Factory aliases   *
     *                   *
     *********************/

    template <std::size_t N, RationalType R = Rational<>>
    using make_rational_pack_t =
        typename details::make_rational_pack<N, R>::type;

    template <std::size_t N>
    using make_pow_ratio_pack_t =
        typename details::make_pow_ratio_pack<N>::type;

    /** Create a RatioPack of size N with a single ratio R at index Idx. */
    template <
        size_t       N,
        size_t       Idx,
        RationalType R       = Rational<1>,
        RationalType Default = Rational<>>
    using make_single_rational_pack_t =
        details::make_single_rational_pack<N, Idx, R, Default>::type;

    // template <RatioPackType R, IntegerPackType I>
    // requires(I::size == R::size && I::size > 0 &&
    //          is_pow_ratio_v<typename R::template type_at<0>>)
    // using make_pow_ratio_pack_t =
    //     details::make_pow_ratio_pack_impl<R, I>::type;   // TODO: implement
    //     this

    /**
     * @brief check if a ratio is a power ratio pack or a standard ratio pack
     *
     * @tparam R
     */
    template <RatioPackType R>
    constexpr bool is_pow_ratio_pack_v =
        is_pow_ratio_v<typename R::template type_at<0>>;

}   // namespace mstd

#endif   // __MSTD_RATIO_PACK_OPERATIONS_HPP__