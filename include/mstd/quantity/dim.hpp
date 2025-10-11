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

#ifndef __MSTD_UNITS_DIMENSION_HPP__
#define __MSTD_UNITS_DIMENSION_HPP__

#include <cstddef>

#include "dim_details.hpp"
#include "enums.hpp"
#include "mstd/pack.hpp"

/**
 * @file dim.hpp
 * @brief Core dimension type and compile-time operations.
 *
 * Defines the `dim` type representing physical dimensions as exponent packs
 * and provides utilities to combine and transform them at compile time.
 */

namespace mstd
{

    /*************************
     *                       *
     * Default integer packs *
     *                       *
     *************************/

    /**
     * @brief Integer pack covering all SI base dimensions.
     *
     * Alias of `make_default_integer_pack_t<SIDimIdMeta::size>`.
     */
    using default_si_pack = make_default_integer_pack_t<SIDimIdMeta::size>;

    /**
     * @brief Integer pack covering all extra (non-SI) dimensions.
     *
     * Alias of `make_default_integer_pack_t<ExtraDimIdMeta::size>`.
     */
    using default_extra_pack =
        make_default_integer_pack_t<ExtraDimIdMeta::size>;

    /***********************
     *                     *
     * The dimension class *
     *                     *
     ***********************/

    /**
     * @brief Encodes a physical dimension via exponent packs.
     *
     * @details Two integer packs store exponents for:
     * - SI base dimensions (7 entries): length, mass, time, electric current,
     *   temperature, amount of substance, luminous intensity
     * - Extra dimensions (4 entries): angle, currency, information, count
     *
     * Packs are `details::integer_pack` specializations providing
     * compile-time sequence access and arithmetic.
     *
     * @tparam SI Pack of SI exponents; defaults to all zeros.
     * @tparam Extra Pack of extra exponents; defaults to all zeros.
     */
    template <
        details::IntegerPack SI    = default_si_pack,
        details::IntegerPack Extra = default_extra_pack>
    struct dim
    {
        static_assert(SI::size == SIDimIdMeta::size, "si size mismatch");
        static_assert(Extra::size == ExtraDimIdMeta::size, "ex size mismatch");

        /** Pack of SI exponents. */
        using si = SI;
        /** Pack of extra exponents. */
        using ex = Extra;

        /** Number of SI dimensions represented. */
        static constexpr size_t si_size = SI::size;
        /** Number of extra dimensions represented. */
        static constexpr size_t ex_size = Extra::size;

        /**
         * @brief Exponent of a specific SI dimension.
         *
         * @tparam ID Enum value identifying the SI dimension.
         */
        template <SIDimId ID>
        static constexpr int si_exp =
            SI::template get<static_cast<size_t>(ID)>();

        /**
         * @brief Exponent of a specific extra dimension.
         *
         * @tparam ID Enum value identifying the extra dimension.
         */
        template <ExtraDimId ID>
        static constexpr int ex_exp =
            Extra::template get<static_cast<size_t>(ID)>();
    };

    /*****************************
     *                           *
     * Dimension operations API  *
     *                           *
     *****************************/

    /**
     * @brief Combine dimensions by adding exponents (multiplication of units).
     *
     * @tparam Dim1 Left operand dimension.
     * @tparam Dim2 Right operand dimension.
     */
    template <details::DimType Dim1, details::DimType Dim2>
    using dim_mul_t =
        dim<pack_add_t<typename Dim1::si, typename Dim2::si>,
            pack_add_t<typename Dim1::ex, typename Dim2::ex>>;

    /**
     * @brief Combine dimensions by subtracting exponents (division of units).
     *
     * @tparam Dim1 Numerator dimension.
     * @tparam Dim2 Denominator dimension.
     */
    template <details::DimType Dim1, details::DimType Dim2>
    using dim_div_t =
        dim<pack_sub_t<typename Dim1::si, typename Dim2::si>,
            pack_sub_t<typename Dim1::ex, typename Dim2::ex>>;

    /**
     * @brief Raise a dimension to an integer power.
     *
     * @tparam Dim Base dimension.
     * @tparam Exp Integer exponent (can be negative).
     */
    template <details::DimType Dim, int Exp>
    using dim_pow_t =
        dim<pack_scale_t<typename Dim::si, Exp>,
            pack_scale_t<typename Dim::ex, Exp>>;

}   // namespace mstd

#endif   // __MSTD_UNITS_DIMENSION_HPP__
