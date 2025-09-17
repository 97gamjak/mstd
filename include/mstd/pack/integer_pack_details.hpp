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

#ifndef __MSTD_INTEGER_PACK_DETAILS_HPP__
#define __MSTD_INTEGER_PACK_DETAILS_HPP__

#include <array>
#include <cstddef>

#include "mstd/functional.hpp"

/**
 * @file integer_pack_details.hpp
 * @brief Internal algorithms and traits for integer_pack.
 *
 * Contains map/zip implementations, factory helpers, and traits/concepts used
 * by the public `integer_pack` interface and its aliases.
 */

namespace mstd::pack
{
    template <int... T>
    struct integer_pack;

    namespace details
    {
        /*******************
         *                 *
         * Pack algorithms  *
         *                 *
         *******************/
        /**
         * @brief Map a function over the elements of a integer_pack.
         *
         * @tparam P The integer_pack to operate on.
         * @tparam F The function to apply.
         * @tparam I The index sequence for unpacking the integer_pack.
         * @return constexpr auto The resulting integer_pack after applying the
         * function.
         */
        template <class P, class F, size_t... I>
        constexpr auto pack_map_impl(std::index_sequence<I...>)
        {
            F f{};
            return integer_pack<f(P::vals[I])...>{};
        }

        /**
         * @brief Implementation note
         *
         * Uses an index sequence to access `P::vals[I]` and builds a new
         * `integer_pack` with the mapped values.
         */

        /**
         * @brief Zip two packs together using a function.
         *
         * @tparam A The first integer_pack.
         * @tparam B The second integer_pack.
         * @tparam F The function to apply.
         * @tparam I The index sequence for unpacking the packs.
         * @return constexpr auto The resulting integer_pack after applying the
         * function.
         */
        template <class A, class B, class F, size_t... I>
        constexpr auto pack_zip_impl(std::index_sequence<I...>)
        {
            static_assert(
                A::size == B::size,
                "integer_pack size mismatch in zip"
            );
            F f{};
            return integer_pack<f(A::vals[I], B::vals[I])...>{};
        }

        /****************************************
         *                                      *
         * Useful aliases for common operations *
         *                                      *
         ****************************************/

        /**
         * @brief Useful alias for mapping a function over a integer_pack.
         *
         * @tparam P The integer_pack to operate on.
         * @tparam F The function to apply.
         */
        template <class P, class F>
        using pack_map_t =
            decltype(pack_map_impl<P, F>(std::make_index_sequence<P::size>{}));

        /**
         * @brief Useful alias for zipping two packs together using a function.
         *
         * @tparam A The first integer_pack.
         * @tparam B The second integer_pack.
         * @tparam F The function to apply.
         */
        template <class A, class B, class F>
        using pack_zip_t =
            decltype(pack_zip_impl<A, B, F>(std::make_index_sequence<A::size>{})
            );

        /*********************
         *                   *
         * Pack factories    *
         *                   *
         *********************/
        /**
         * @brief Build a zero-initialized `integer_pack` of size N.
         */
        template <std::size_t N, typename Seq = std::make_index_sequence<N>>
        struct make_default_integer_pack;

        template <std::size_t N, std::size_t... Is>
        struct make_default_integer_pack<N, std::index_sequence<Is...>>
        {
            using type = integer_pack<(static_cast<void>(Is), 0)...>;
        };

        /**
         * @brief Build an `integer_pack` of size N with a single non-zero.
         *
         * Sets the element at index `Ix` to value `I` (default 1); all others
         * are zero.
         */
        template <
            std::size_t N,
            std::size_t Ix,
            int         I = 1,
            typename Seq  = std::make_index_sequence<N>>
        struct make_single_integer_pack;

        template <std::size_t N, std::size_t Ix, int I, std::size_t... Is>
        struct make_single_integer_pack<N, Ix, I, std::index_sequence<Is...>>
        {
            using type = integer_pack<(Is == Ix ? I : 0)...>;
        };

        /********************
         *                  *
         * Traits & concept *
         *                  *
         ********************/
        /**
         * @brief Trait to detect `integer_pack<...>` types.
         */
        template <class>
        struct is_integer_pack : std::false_type
        {
        };

        template <int... Es>
        struct is_integer_pack<integer_pack<Es...>> : std::true_type
        {
        };

        /**
         * @brief Concept satisfied by `integer_pack` specializations.
         */
        template <class T>
        concept IntegerPack = details::is_integer_pack<T>::value;

    }   // namespace details

}   // namespace mstd::pack

#endif   // __MSTD_INTEGER_PACK_DETAILS_HPP__
