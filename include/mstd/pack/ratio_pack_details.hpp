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

#ifndef __MSTD_RATIO_PACK_DETAILS_HPP__
#define __MSTD_RATIO_PACK_DETAILS_HPP__

#include <array>
#include <cstddef>
#include <ratio>
#include <tuple>

#include "mstd/ratio.hpp"
#include "ratio_pack.hpp"

/**
 * @file ratio_pack_details.hpp
 * @brief Internal algorithms, factories, and traits for ratio_pack.
 *
 * Implements element-wise operations over packs of `std::ratio` types, pack
 * construction helpers, and detection traits used by the public interface.
 */

namespace mstd::details
{
    /*******************
     *                 *
     * Pack algorithms  *
     *                 *
     *******************/
    template <class A, class B, template <class, class> class F, size_t... I>
    constexpr auto ratio_pack_zip_impl(std::index_sequence<I...>)
    {
        static_assert(A::size == B::size, "RatioPack size mismatch");
        return RatioPack<typename F<
            typename A::template type_at<I>,
            typename B::template type_at<I>>::type...>{};
    }

    template <class A, class B, template <class, class> class F>
    using ratio_pack_zip_t = decltype(ratio_pack_zip_impl<A, B, F>(
        std::make_index_sequence<A::size>{}
    ));

    template <class Pack, IntegerPackType IntPack>
    struct ratio_pack_pow_impl
    {
        static_assert(
            Pack::size == IntPack::size,
            "RatioPack and IntegerPack size mismatch"
        );
        template <class Seq>
        struct _impl;

        // expand over indices
        template <size_t... I>
        struct _impl<std::index_sequence<I...>>
        {
            // REQUIRE: IntPack::template get<I> is a static constexpr int
            // (a value), not a function
            using type = RatioPack<ratio_pow_t<
                typename Pack::template type_at<I>,
                IntPack::template get<I>>...>;
        };

        // produce the final type by instantiating the helper with
        // 0..Pack::size-1
        using type = typename _impl<std::make_index_sequence<Pack::size>>::type;
    };

    // ---- apply power to every entry in a RatioPack
    template <class Pack, int K>
    struct ratio_pack_pow_k_impl
    {
        using type =
            ratio_pack_pow_impl<Pack, make_integer_pack_t<Pack::size, K>>::type;
    };

    /*********************
     *                   *
     * Value computation *
     *                   *
     *********************/
    template <class Pack, size_t... I>
    consteval long double ratio_pack_value_impl(std::index_sequence<I...>)
    {
        auto r = static_cast<long double>(1);
        // fold over compile-time indices
        ((r *= Pack::template get<I>()), ...);
        return r;
    }

    /*********************
     *                   *
     * Pack factories    *
     *                   *
     *********************/
    // helper: always yields std::ratio<1>
    template <size_t>   // TODO: move this to ratio
    using default_ratio = std::ratio<1>;

    template <size_t>   // TODO: move this to powratio
    using default_pow_ratio = PowRatio<>;

    // main generator
    template <size_t N, typename Seq = std::make_index_sequence<N>>
    struct make_default_ratio_pack;

    template <size_t N, size_t... Is>
    struct make_default_ratio_pack<N, std::index_sequence<Is...>>
    {
        using type = RatioPack<default_ratio<Is>...>;
    };

    template <size_t N, typename Seq = std::make_index_sequence<N>>
    struct make_pow_ratio_pack;

    template <size_t N, size_t... Is>
    struct make_pow_ratio_pack<N, std::index_sequence<Is...>>
    {
        using type = RatioPack<default_pow_ratio<Is>...>;
    };

    // overload taking the actual R
    template <class R, size_t Idx, size_t... I>
    constexpr auto make_ratio_pack_at_impl(std::index_sequence<I...>)
    {
        return RatioPack<std::conditional_t<I == Idx, R, std::ratio<1>>...>{};
    }

    template <typename R, typename I, typename Seq>
    struct make_pow_ratio_pack_impl
    {
        MSTD_COMPILE_FAIL(
            "making a power ratio pack with the given template args is not "
            "supported"
        );
    }

    template <RatioPackType R, IntegerPackType I, size_t... I>
    requires(
        I::size == R::size && I::size > 0 &&
        is_pow_ratio_v<typename R::type_at<0>>
    )
    struct make_pow_ratio_pack_impl<
        R,
        I,
        std::index_sequence<I...>>   // TODO: needs a second integer pack
    {
        using type = RatioPack<ratio_pow_t<
            typename R::template type_at<I>,
            I::template get<I>>...>;
    };

}   // namespace mstd::details

#endif   // __MSTD_RATIO_PACK_DETAILS_HPP__
