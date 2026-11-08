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

#ifndef __MSTD__MATH__POWER_HPP__
#define __MSTD__MATH__POWER_HPP__

#include <cmath>
#include <cstdint>
#include <ratio>
#include <type_traits>

#include "adl.hpp"

namespace mstd
{

    /**
     * @brief constexpr power helper using exponentiation by squaring.
     *
     * The exponent is a template argument which allows the compiler to fold the
     * entire expression whenever `base` is also `constexpr`. Special cases are
     * handled explicitly to avoid unnecessary recursion.
     *
     * @tparam N compile-time exponent (can be negative).
     * @tparam T arithmetic type supporting multiplication and reciprocal.
     * @param base value raised to the power @p N.
     */
    template <intmax_t N, typename T>
    static inline constexpr T cpow(const T base)
    {
        if constexpr (N < 0)
            return static_cast<T>(1) / cpow<-N>(base);
        else if constexpr (N == 1)
            return base;
        else if constexpr (N == 0)
            return static_cast<T>(1);
        else if constexpr (N % 2 == 0)
            return cpow<N / 2>(base) * cpow<N / 2>(base);
        else   // (N % 2 == 1)
            return cpow<N / 2>(base) * cpow<N / 2>(base) * base;
    }

    /**
     * @brief Runtime variant of cpow for integral exponents.
     *
     * Provides a lightweight alternative to `std::pow` while keeping the
     * implementation dependency free. Negative exponents are supported by
     * inverting the base.
     */
    template <typename T>
    [[deprecated]] T power(T base, int exponent)
    {
        T result = static_cast<T>(1);

        if (exponent < 0)
        {
            base     = 1 / base;
            exponent = -exponent;
        }

        for (int i = 0; i < exponent; i++)
            result *= base;

        return result;
    }

}   // namespace mstd

namespace mstd::detail
{
    using std::cbrt;
    using std::sqrt;

    // fallbacks for arithmetic types
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto sqrt_impl(const T &x, adl_tag)
    {
        return sqrt(x);
    }
    template <typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto cbrt_impl(const T &x, adl_tag)
    {
        return cbrt(x);
    }
    template <std::intmax_t Num, std::intmax_t Den = 1, typename T>
    requires std::is_arithmetic_v<T>
    constexpr auto pow_impl(const T &x, adl_tag)
    {
        if constexpr (Den == 1)
            return cpow<Num>(static_cast<std::common_type_t<T, double>>(x));
        else
            return std::pow(
                static_cast<std::common_type_t<T, double>>(x),
                static_cast<double>(Num) / static_cast<double>(Den)
            );
    }
}   // namespace mstd::detail

namespace mstd
{
    // unqualified calls: ADL (through adl_tag) finds the hooks, including
    // the quantity overloads units.hpp adds later
    template <typename T>
    constexpr auto sqrt(const T &x)
    {
        return sqrt_impl(x, detail::adl_tag{});
    }
    template <typename T>
    constexpr auto cbrt(const T &x)
    {
        return cbrt_impl(x, detail::adl_tag{});
    }
    /// x raised to the compile-time power Num/Den
    template <std::intmax_t Num, std::intmax_t Den = 1, typename T>
    constexpr auto pow(const T &x)
    {
        return pow_impl<Num, Den>(x, detail::adl_tag{});
    }
}   // namespace mstd

#endif   // __MSTD__MATH__POWER_HPP__
