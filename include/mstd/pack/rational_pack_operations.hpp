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

    template <RatioPackType A, RatioPackType B>
    struct add_type<A, B>
    {
        using type = details::ratio_pack_zip_t<A, B, add_type_t>;
    };

    template <RatioPackType A, RatioPackType B>
    struct sub_type<A, B>
    {
        using type = details::ratio_pack_zip_t<A, B, sub_type_t>;
    };

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

    template <typename R>
    struct to_pow_ratio_pack_impl
    {
        /**
         * @brief Build an `RationalPack` of size N filled with `rational`.
         *
         * @tparam Is A sequence of indices.
         */
        template <typename Seq>
        struct _impl;

        template <size_t... Is>
        struct _impl<std::index_sequence<Is...>>
        {
            template <size_t I>
            using repeat_t = to_pow_rational_t<typename R::template type_at<I>>;

            using type = RationalPack<repeat_t<Is>...>;
        };

        using type = typename _impl<std::make_index_sequence<R::size>>::type;
    };

    template <typename R>
    using to_pow_ratio_pack_t = to_pow_ratio_pack_impl<R>::type;

    template <std::size_t N, RationalType R = Rational<>>
    using make_rational_pack_t =
        typename details::make_rational_pack<N, R>::type;

    template <size_t N, RationalType R = Rational<1>>
    using make_ratio_pack_t = typename details::make_rational_pack<N, R>::type;

    template <std::size_t N, RationalType R>
    struct make_pow_rational_pack   // TODO: implement version for RationalPower
    {
        using type = to_pow_ratio_pack_t<make_ratio_pack_t<N, R>>;
    };

    template <std::size_t N, typename R = Rational<1>>
    using make_pow_ratio_pack_t = typename make_pow_rational_pack<N, R>::
        type;   // TODO: implement version for RationalPower

    /** Create a RatioPack of size N with a single ratio R at index Idx. */
    template <
        size_t       N,
        size_t       Idx,
        RationalType R       = Rational<1>,
        RationalType Default = Rational<>>
    using make_single_rational_pack_t =
        details::make_single_rational_pack<N, Idx, R, Default>::type;

    template <
        size_t       N,
        size_t       Idx,
        RationalType R       = Rational<>,
        RationalType Default = Rational<>>
    using make_single_rational_pow_pack_t =
        details::make_single_rational_pow_pack<N, Idx, R, Default>::
            type;   // TODO: implement version for RationalPower

    /**
     * @brief check if a ratio is a power ratio pack or a standard ratio
     * pack
     *
     * @tparam R
     */
    template <RatioPackType R>
    constexpr bool is_pow_ratio_pack_v =
        is_pow_ratio_v<typename R::template type_at<0>>;

}   // namespace mstd

#endif   // __MSTD_RATIO_PACK_OPERATIONS_HPP__