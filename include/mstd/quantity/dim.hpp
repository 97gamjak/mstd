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
     * Alias of `make_rational_pack_t<SIDimIdMeta::size>`.
     */
    using default_si_pack = make_rational_pack_t<SIDimIdMeta::size>;

    /**
     * @brief Integer pack covering all extra (non-SI) dimensions.
     *
     * Alias of `make_rational_pack_t<ExtraDimIdMeta::size>`.
     */
    using default_extra_pack = make_rational_pack_t<ExtraDimIdMeta::size>;

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
     * Packs are `details::` specializations providing
     * compile-time sequence access and arithmetic.
     *
     * @tparam SI Pack of SI exponents; defaults to all zeros.
     * @tparam Extra Pack of extra exponents; defaults to all zeros.
     */
    template <
        typename SI    = default_si_pack,
        typename Extra = default_extra_pack>
    requires(RationalPackType<SI> || IntegerPackType<SI>) &&
            (RationalPackType<Extra> || IntegerPackType<Extra>)
    struct Dim
    {
        static_assert(SI::size == SIDimIdMeta::size, "si size mismatch");
        static_assert(Extra::size == ExtraDimIdMeta::size, "ex size mismatch");

        // pack of SI exponents
        // NOTE: convert IntegerPack to RationalPack if needed
        using si = convert_to_rational_pack_t<SI>;

        // pack of extra exponents
        // NOTE: convert IntegerPack to RationalPack if needed
        using ex = convert_to_rational_pack_t<Extra>;

        /** Number of SI dimensions represented. */
        static constexpr size_t si_size = si::size;
        /** Number of extra dimensions represented. */
        static constexpr size_t ex_size = ex::size;

        /**
         * @brief Exponent of a specific SI dimension.
         *
         * @tparam ID Enum value identifying the SI dimension.
         */
        template <SIDimId ID>
        static constexpr long double si_exp_v = si::template get<ID>();

        /**
         * @brief Exponent of a specific extra dimension.
         *
         * @tparam ID Enum value identifying the extra dimension.
         */
        template <ExtraDimId ID>
        static constexpr long double ex_exp_v = ex::template get<ID>();
    };

    template <DimType D1, DimType D2>
    struct is_same<D1, D2>
    {
        static constexpr bool value =
            std::is_same_v<typename D1::si, typename D2::si> &&
            std::is_same_v<typename D1::ex, typename D2::ex>;
    };

}   // namespace mstd

#endif   // __MSTD_UNITS_DIMENSION_HPP__
