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

/**
 * @file ratio_pack_details.hpp
 * @brief Internal algorithms, factories, and traits for ratio_pack.
 *
 * Implements element-wise operations over packs of `std::ratio` types, pack
 * construction helpers, and detection traits used by the public interface.
 */

namespace mstd
{
    template <class... Rs>
    struct ratio_pack;

    namespace details
    {
        /********************
         *                  *
         * Traits & concept *
         *                  *
         ********************/
        template <class>
        struct is_ratio_pack : std::false_type
        {
        };

        template <class... Rs>
        struct is_ratio_pack<ratio_pack<Rs...>> : std::true_type
        {
        };

        template <class T>
        concept RatioPack = details::is_ratio_pack<T>::value;

        /*******************
         *                 *
         * Pack algorithms  *
         *                 *
         *******************/
        template <
            class A,
            class B,
            template <class, class> class F,
            size_t... I>
        constexpr auto ratio_pack_zip_impl(std::index_sequence<I...>)
        {
            static_assert(A::size == B::size, "ratio_pack size mismatch");
            return ratio_pack<typename F<
                typename A::template type_at<I>,
                typename B::template type_at<I>>::type...>{};
        }

        template <class A, class B, template <class, class> class F>
        using ratio_pack_zip_t = decltype(ratio_pack_zip_impl<A, B, F>(
            std::make_index_sequence<A::size>{}
        ));

        // ---- apply power to every entry in a ratio_pack
        template <class Pack, int K, size_t... I>
        constexpr auto ratio_pack_pow_impl(std::index_sequence<I...>)
        {
            return ratio_pack<
                ratio_pow_t<typename Pack::template type_at<I>, K>...>{};
        }

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

        template <class Pack>
        inline constexpr long double ratio_pack_v =
            details::ratio_pack_value_impl<Pack>(
                std::make_index_sequence<Pack::size>{}
            );

        /*********************
         *                   *
         * Pack factories    *
         *                   *
         *********************/
        // helper: always yields std::ratio<1>
        template <std::size_t>
        using default_ratio = std::ratio<1>;

        // main generator
        template <std::size_t N, typename Seq = std::make_index_sequence<N>>
        struct make_default_ratio_pack;

        template <std::size_t N, std::size_t... Is>
        struct make_default_ratio_pack<N, std::index_sequence<Is...>>
        {
            using type = ratio_pack<default_ratio<Is>...>;
        };

        /**
         * @brief Build a ratio_pack of size N with ratio R at index Idx.
         */
        template <size_t Idx, size_t... I>
        constexpr auto make_ratio_pack_at_impl(std::index_sequence<I...>)
        {
            return ratio_pack<
                std::
                    conditional_t<I == Idx, std::ratio<1>, std::ratio<1>>...>{};
        }
        // overload taking the actual R
        template <class R, size_t Idx, size_t... I>
        constexpr auto make_ratio_pack_at_impl_R(std::index_sequence<I...>)
        {
            return ratio_pack<
                std::conditional_t<I == Idx, R, std::ratio<1>>...>{};
        }

    }   // namespace details

}   // namespace mstd

#endif   // __MSTD_RATIO_PACK_DETAILS_HPP__
