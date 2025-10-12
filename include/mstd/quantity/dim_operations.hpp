#ifndef __MSTD_DIM_OPERATIONS_HPP__
#define __MSTD_DIM_OPERATIONS_HPP__

#include "dim.hpp"
#include "dim_details.hpp"

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
    using dim_mul_t =
        dim<pack_add_t<typename Dim1::si, typename Dim2::si>,
            pack_add_t<typename Dim1::ex, typename Dim2::ex>>;

    /**
     * @brief Combine dimensions by subtracting exponents (division of units).
     *
     * @tparam Dim1 Numerator dimension.
     * @tparam Dim2 Denominator dimension.
     */
    template <DimType Dim1, DimType Dim2>
    using dim_div_t =
        dim<pack_sub_t<typename Dim1::si, typename Dim2::si>,
            pack_sub_t<typename Dim1::ex, typename Dim2::ex>>;

    /**
     * @brief Raise a dimension to an integer power.
     *
     * @tparam Dim Base dimension.
     * @tparam Exp Integer exponent (can be negative).
     */
    template <DimType Dim, int Exp>
    using dim_pow_t =
        dim<pack_scale_t<typename Dim::si, Exp>,
            pack_scale_t<typename Dim::ex, Exp>>;

}   // namespace mstd

#endif   // __MSTD_DIM_OPERATIONS_HPP__