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

#include "mstd/functional.hpp"

namespace mstd::pack
{
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

        static constexpr size_t num_vals_set()
        {
            size_t count = 0;
            for (size_t i = 0; i < size; ++i)
                if (vals[i] != 0)
                    ++count;
            return count;
        }

        static constexpr size_t non_zero_index()
        {
            for (size_t i = 0; i < size; ++i)
                if (vals[i] != 0)
                    return i;

            return -1;
        }
    };

    // ---------------------------
    // zip and map for ratio_pack
    // ---------------------------

    namespace details
    {

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

    }   // namespace details

    // Useful aliases for common operations
    template <class A, class B>
    using pack_add_t = details::pack_zip_t<A, B, functional::add_fn>;
    template <class A, class B>
    using pack_sub_t = details::pack_zip_t<A, B, functional::sub_fn>;
    template <class P, int K>
    using pack_scale_t = details::pack_map_t<P, functional::scale_fn<K>>;

    // primary template
    template <std::size_t N, typename Seq = std::make_index_sequence<N>>
    struct make_default_integer_pack;

    // specialization with index sequence
    template <std::size_t N, std::size_t... Is>
    struct make_default_integer_pack<N, std::index_sequence<Is...>>
    {
        using type = integer_pack<(static_cast<void>(Is), 0)...>;
    };

    // convenient alias
    template <std::size_t N>
    using make_default_integer_pack_t =
        typename make_default_integer_pack<N>::type;

    namespace details
    {
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

    }   // namespace details

    // helper alias
    template <std::size_t N, std::size_t Ix, int I = 1>
    using make_single_integer_pack_t =
        typename details::make_single_integer_pack<N, Ix, I>::type;

    namespace details
    {

        template <class>
        struct is_integer_pack : std::false_type
        {
        };

        template <int... Es>
        struct is_integer_pack<integer_pack<Es...>> : std::true_type
        {
        };

        template <class T>
        concept IntegerPack = details::is_integer_pack<T>::value;
    }   // namespace details

}   // namespace mstd::pack

#endif   // __MSTD_INTEGER_PACK_HPP__