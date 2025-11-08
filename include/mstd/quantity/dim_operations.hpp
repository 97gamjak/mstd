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

#ifndef __MSTD_DIM_OPERATIONS_HPP__
#define __MSTD_DIM_OPERATIONS_HPP__

#include "dim.hpp"
#include "dim_details.hpp"
#include "mstd/type_traits.hpp"

/**
 * @file dim_operations.hpp
 * @brief Compile-time utilities for dimension algebra.
 *
 * Provides helpers to combine dimensions (multiply, divide, power) and
 * check dimensional compatibility.
 */

namespace mstd
{
    /**
     * @brief Combine dimensions by adding exponents (multiplication of units).
     *
     * @tparam Dim1 Left operand dimension.
     * @tparam Dim2 Right operand dimension.
     */
    template <DimType Dim1, DimType Dim2>
    struct mul_type<Dim1, Dim2>
    {
        using type =
            Dim<add_type_t<typename Dim1::si, typename Dim2::si>,
                add_type_t<typename Dim1::ex, typename Dim2::ex>>;
    };

    /**
     * @brief Combine dimensions by subtracting exponents (division of units).
     *
     * @tparam Dim1 Numerator dimension.
     * @tparam Dim2 Denominator dimension.
     */
    template <DimType Dim1, DimType Dim2>
    struct div_type<Dim1, Dim2>
    {
        using type =
            Dim<sub_type_t<typename Dim1::si, typename Dim2::si>,
                sub_type_t<typename Dim1::ex, typename Dim2::ex>>;
    };

    /**
     * @brief Raise a dimension to an integer power.
     *
     * @tparam Dim Base dimension.
     * @tparam Exp Integer exponent (can be negative).
     */
    template <DimType D, int Exp>
    using dim_pow_t =
        Dim<pack_scale_t<typename D::si, Exp>,
            pack_scale_t<typename D::ex, Exp>>;

}   // namespace mstd

#endif   // __MSTD_DIM_OPERATIONS_HPP__