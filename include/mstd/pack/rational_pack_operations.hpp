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

    template <
        template <typename...> typename F,
        typename Pack0,
        typename... Packs>
    requires((RationalPackType<Packs> || RationalPowerPackType<Packs>) && ...
            ) &&
            (RationalPackType<Pack0> || RationalPowerPackType<Pack0>)
    struct zip_type<F, Pack0, Packs...>
    {
        using type = details::ratio_pack_zip_impl<
            F,
            Pack0,
            std::make_index_sequence<Pack0::size>,
            Packs...>::type;
    };

    // Specialization of reduce_type for the (F, Pack) form:
    template <
        template <typename, typename> typename F,
        typename Init,
        typename Pack>
    requires(RationalPackType<Pack> || RationalPowerPackType<Pack>)
    struct reduce_type<F, Init, Pack>
    {
        using type = details::ratio_pack_reduce_t<F, Init, Pack>;
    };

    /*************************
     *                       *
     * Convenience aliases   *
     *                       *
     *************************/

    template <RationalPackType A, RationalPackType B>
    struct add_type<A, B>
    {
        using type = zip_type_t<add_type_t, A, B>;
    };

    template <RationalPackType A, RationalPackType B>
    struct sub_type<A, B>
    {
        using type = zip_type_t<sub_type_t, A, B>;
    };

    /** Element-wise multiply two ratio packs. */
    template <typename A, typename B>
    requires(RationalPackType<A> || RationalPowerPackType<A>) &&
            (RationalPackType<B> || RationalPowerPackType<B>)
    struct mul_type<A, B>
    {
        using type = zip_type_t<mul_type_t, A, B>;
    };

    /** Element-wise divide two ratio packs. */
    template <typename A, typename B>
    requires(RationalPackType<A> || RationalPowerPackType<A>) &&
            (RationalPackType<B> || RationalPowerPackType<B>)
    struct div_type<A, B>
    {
        using type = zip_type_t<div_type_t, A, B>;
    };

    /** Raise each ratio in a pack to integer powers from an IntegerPack. */
    template <
        RationalPackType Pack,
        IntegerPackType  IntPack>   // TODO: check this
    struct pow_type<Pack, IntPack>
    {
        using type = details::ratio_pack_pow_impl<Pack, IntPack>::type;
    };

    /** Raise each ratio in a pack to integer power K. */
    template <RationalPackType Pack, int K>   // TODO: check this
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

    template <std::size_t N, typename R = Rational<>>
    requires(RationalType<R> || RationalPowerType<R>)
    using make_rational_pack_t =
        typename details::make_rational_pack<N, R>::type;

    template <size_t N, typename R = Rational<1>>
    requires(RationalType<R> || RationalPowerType<R>)
    using make_ratio_pack_t = typename details::make_rational_pack<N, R>::type;

    template <std::size_t N, RationalPowerType R>
    struct make_pow_rational_pack
    {
        using type = make_ratio_pack_t<N, R>;
    };

    template <std::size_t N, RationalPowerType R = RationalPower<>>
    using make_pow_ratio_pack_t = typename make_pow_rational_pack<N, R>::type;

    /** Create a RatioPack of size N with a single ratio R at index Idx. */
    template <
        size_t       N,
        size_t       Idx,
        RationalType R       = Rational<1>,
        RationalType Default = Rational<>>
    using make_single_rational_pack_t =
        details::make_single_rational_pack<N, Idx, R, Default>::type;

    template <
        size_t            N,
        size_t            Idx,
        RationalPowerType R,
        RationalPowerType Default>
    using make_single_rational_pow_pack_t =
        details::make_single_rational_pow_pack<N, Idx, R, Default>::type;

    /**
     * @brief check if a ratio is a power ratio pack or a standard ratio
     * pack
     *
     * @tparam R
     */
    template <RationalPackType R>
    constexpr bool is_pow_ratio_pack_v =
        is_rational_power_v<typename R::template type_at<0>>;

    template <typename T>
    struct convert_to_rational_pack
    {
        MSTD_COMPILE_FAIL("Unsupported type for converting to rational pack");
    };

    template <RationalPackType R>
    struct convert_to_rational_pack<R>
    {
        using type = R;
    };

    template <IntegerPackType IntPack>
    struct convert_to_rational_pack<IntPack>
    {
        /**
         * @brief Build a `RationalPack` from an `IntegerPack`.
         *
         * @tparam Is A sequence of indices.
         */
        template <typename Seq>
        struct _impl;

        template <size_t... Is>
        struct _impl<std::index_sequence<Is...>>
        {
            using type = RationalPack<Rational<IntPack::template get<Is>>...>;
        };

        using type =
            typename _impl<std::make_index_sequence<IntPack::size>>::type;
    };

    template <typename IntPack>
    using convert_to_rational_pack_t =
        typename convert_to_rational_pack<IntPack>::type;

}   // namespace mstd

#endif   // __MSTD_RATIO_PACK_OPERATIONS_HPP__