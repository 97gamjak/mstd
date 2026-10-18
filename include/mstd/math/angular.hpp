#ifndef __MSTD__MATH__ANGULAR_HPP__
#define __MSTD__MATH__ANGULAR_HPP__

// mstd/math.hpp
#pragma once
#include <cmath>
#include <type_traits>

namespace mstd::detail
{
    // These declarations stop unqualified lookup inside detail, so the
    // *_impl calls can't find mstd::cos etc. and recurse into themselves.
    using std::acos;
    using std::asin;
    using std::atan;
    using std::atan2;
    using std::cos;
    using std::sin;
    using std::tan;

    /**
     * @brief Convert a quantity to its scalar numerical value.
     *
     * If the input is an arithmetic type, it is returned unchanged.
     * Otherwise, it is assumed to be a quantity with a unit, and its
     * numerical value in its own unit is returned.
     *
     * @tparam T The type of the input quantity or scalar.
     * @param r The input quantity or scalar.
     *
     * @return The scalar numerical value of the input.
     */
    template <typename T>
    constexpr auto to_scalar(const T& r)
    {
        if constexpr (std::is_arithmetic_v<T>)
            return r;
        else
            return r.numerical_value_in(r.unit);
    }
}   // namespace mstd::detail

#define MSTD_MATH_UNARY(fn)                                 \
    namespace detail                                        \
    {                                                       \
        template <typename T>                               \
        constexpr auto fn##_impl(const T& x)                \
        {                                                   \
            return fn(x);                                   \
        } /* ADL */                                         \
    }                                                       \
    template <typename T>                                   \
    requires requires(const T& x) { detail::fn##_impl(x); } \
    constexpr auto fn(const T& x)                           \
    {                                                       \
        return detail::to_scalar(detail::fn##_impl(x));     \
    }

namespace mstd
{
    MSTD_MATH_UNARY(sin)
    MSTD_MATH_UNARY(cos)
    MSTD_MATH_UNARY(tan)
    MSTD_MATH_UNARY(asin)
    MSTD_MATH_UNARY(acos)
    MSTD_MATH_UNARY(atan)

    namespace detail
    {
        /**
         * @brief Compute the arctangent of y/x considering the signs of both
         * arguments.
         *
         * @tparam Y The type of the y-coordinate.
         * @tparam X The type of the x-coordinate.
         * @param y The y-coordinate.
         * @param x The x-coordinate.
         *
         * @return The arctangent of y/x in radians.
         */
        template <typename Y, typename X>
        constexpr auto atan2_impl(const Y& y, const X& x)
        {
            return atan2(y, x);
        }
    }   // namespace detail

    /**
     * @brief Compute the arctangent of y/x considering the signs of both
     * arguments.
     *
     * @tparam Y The type of the y-coordinate.
     * @tparam X The type of the x-coordinate.
     * @param y The y-coordinate.
     * @param x The x-coordinate.
     *
     * @return The arctangent of y/x in radians.
     */
    template <typename Y, typename X>
    requires requires(const Y& y, const X& x) { detail::atan2_impl(y, x); }
    constexpr auto atan2(const Y& y, const X& x)
    {
        return detail::to_scalar(detail::atan2_impl(y, x));
    }
}   // namespace mstd

#undef MSTD_MATH_UNARY

#endif   // __MSTD__MATH__ANGULAR_HPP__
