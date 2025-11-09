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

#ifndef __MSTD_PACK_TRAITS_HPP__
#define __MSTD_PACK_TRAITS_HPP__

#include "math_traits.hpp"

/**
 * @file pack_traits.hpp
 * @brief Concepts for integer and ratio pack types.
 *
 * Provides concepts to check if a type conforms to the expected interface
 * of integer packs and ratio packs used in dimension and dimension ratio
 * representations.
 */

namespace mstd
{
    /**
     * @brief Concept for integer pack types.
     *
     * @details An integer pack type must provide a static member `size` and
     * a member template `get<Index>()` returning the integer at position
     * `Index`.
     */
    template <typename T>
    concept IntegerPackType = requires {
        { T::size } -> std::convertible_to<std::size_t>;
        { T::template get<0> } -> std::convertible_to<int>;
    };

    namespace details
    {
        template <typename T, size_t... I>
        constexpr bool is_ratio_pack_ok(std::index_sequence<I...>)
        {
            return (is_rational_v<typename T::template type_at<I>> && ...);
        }

        template <typename T, size_t... I>
        constexpr bool is_pow_ratio_pack_ok(std::index_sequence<I...>)
        {
            return (
                is_rational_power_v<typename T::template type_at<I>> && ...
            );
        }

    }   // namespace details

    /**
     * @brief Concept for ratio pack types.
     *
     * @details A ratio pack type must provide a static member `size` and
     * a member template `get<Index>()` returning the `std::ratio` at
     * position `Index`.
     */
    // template <typename T>
    // concept RatioPackType = requires {
    //     { T::size } -> std::convertible_to<std::size_t>;
    // } && details::is_ratio_pack_ok<T>(std::make_index_sequence<T::size>{});
    // TODO: check if this trait makes even sense, and if so use it without
    // allowing 0s

    template <typename T>
    concept RationalPackType = requires {
        { T::size } -> std::convertible_to<std::size_t>;
    } && requires {
        typename T::template type_at<0>;
    } && details::is_ratio_pack_ok<T>(std::make_index_sequence<T::size>{});

    template <typename T>
    concept RationalPowerPackType = requires {
        { T::size } -> std::convertible_to<std::size_t>;
    } && requires {
        typename T::template type_at<0>;
    } && details::is_pow_ratio_pack_ok<T>(std::make_index_sequence<T::size>{});

    template <typename T>
    constexpr bool is_integer_pack_v = IntegerPackType<T>;

    template <typename T>
    constexpr bool is_rational_pack_v = RationalPackType<T>;

    template <
        template <typename...> typename F,
        typename Pack0,
        typename... Packs>
    struct zip_type
    {
        MSTD_COMPILE_FAIL("zip_type not implemented for the given types");
    };

    template <
        template <typename...> typename F,
        typename Pack0,
        typename... Packs>
    using zip_type_t = typename zip_type<F, Pack0, Packs...>::type;

    template <
        template <typename, typename> typename F,
        typename Init,
        typename... Ts>
    struct reduce_type
    {
        MSTD_COMPILE_FAIL("reduce_type not implemented for the given types");
    };

    template <template <typename, typename> typename F, typename Init>
    struct reduce_type<F, Init>
    {
        using type = Init;
    };

    // Step: fold left
    template <
        template <typename, typename> typename F,
        typename Init,
        typename T0,
        typename... Ts>
    struct reduce_type<F, Init, T0, Ts...>
    {
        using type = typename reduce_type<F, F<Init, T0>, Ts...>::type;
    };

    template <
        template <typename, typename> typename F,
        typename Init,
        typename... Ts>
    using reduce_type_t = typename reduce_type<F, Init, Ts...>::type;

}   // namespace mstd

#endif   // __MSTD_PACK_TRAITS_HPP__