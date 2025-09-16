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

namespace mstd::units
{

    /***********************
     *                     *
     * The dimension class *
     *                     *
     ***********************/

    /**
     * @brief A class representing a physical dimension.
     *
     * @details A dimension is represented by two packs of integers:
     *          - The first pack represents the exponents of the 7 base SI
     * dimensions: length, mass, time, electric current, temperature, amount of
     * substance, luminous intensity.
     *          - The second pack represents the exponents of 4 additional
     * "extra" dimensions: angle, currency, information, count. Each pack is
     * implemented using the `details::integer_pack` class, which provides
     * compile-time storage and manipulation of integer sequences.
     * @tparam SIDimPack   A `details::integer_pack` representing the SI
     * dimension exponents.
     * @tparam ExtraDimPack A `details::integer_pack` representing the extra
     * dimension exponents.
     */
    template <pack::details::IntegerPack SI, pack::details::IntegerPack Extra>
    struct dim
    {
        static_assert(SI::size == SIDimIdMeta::size, "si size mismatch");
        static_assert(Extra::size == ExtraDimIdMeta::size, "ex size mismatch");

        using si = SI;
        using ex = Extra;

        static constexpr size_t si_size = SI::size;
        static constexpr size_t ex_size = Extra::size;

        /**
         * @brief get the exponent of a specific si dimension
         *
         * @tparam ID
         */
        template <SIDimId ID>
        static constexpr int si_exp =
            SI::template get<static_cast<size_t>(ID)>();

        /**
         * @brief get the exponent of a specific extra dimension
         *
         * @tparam ID
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
     * @brief Adds two dimensions.
     *
     * @tparam Dim1 The first dimension type.
     * @tparam Dim2 The second dimension type.
     * @return The resulting dimension after adding the two dimensions.
     */
    template <details::DimType Dim1, details::DimType Dim2>
    using dim_mul_t =
        dim<pack::pack_add_t<typename Dim1::si, typename Dim2::si>,
            pack::pack_add_t<typename Dim1::ex, typename Dim2::ex>>;

    /**
     * @brief Subtracts one dimension from another.
     *
     * @tparam Dim1 The dimension type to subtract from.
     * @tparam Dim2 The dimension type to subtract.
     * @return The resulting dimension after subtraction.
     */
    template <details::DimType Dim1, details::DimType Dim2>
    using dim_div_t =
        dim<pack::pack_sub_t<typename Dim1::si, typename Dim2::si>,
            pack::pack_sub_t<typename Dim1::ex, typename Dim2::ex>>;

    /**
     * @brief Raises a dimension to an integer power.
     *
     * @tparam Dim The dimension type to raise to a power.
     * @tparam Exp The power to raise the dimension to.
     * @return The resulting dimension after raising to the power.
     */
    template <details::DimType Dim, int Exp>
    using dim_pow_t =
        dim<pack::pack_scale_t<typename Dim::si, Exp>,
            pack::pack_scale_t<typename Dim::ex, Exp>>;

}   // namespace mstd::units

#endif   // __MSTD_UNITS_DIMENSION_HPP__