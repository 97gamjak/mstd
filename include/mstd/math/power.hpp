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

#ifndef __MSTD_MATH_POWER_HPP__
#define __MSTD_MATH_POWER_HPP__

#include <ratio>

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

#endif   // __MSTD_MATH_POWER_HPP__
