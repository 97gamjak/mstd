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

#ifndef __MSTD_MATH_RATIONAL_HPP__
#define __MSTD_MATH_RATIONAL_HPP__

#include <numeric>
#include <ratio>

namespace mstd
{
    template <intmax_t Num = 1, intmax_t Den = 1>
    struct Rational
    {
        static constexpr intmax_t rawNum = Num;
        static constexpr intmax_t rawDen = Den;

        static constexpr intmax_t gcd =
            Num == 0 || Den == 0 ? 1 : std::gcd(Num, Den);

        static constexpr intmax_t num = rawNum / gcd;
        static constexpr intmax_t den = rawDen / gcd;

        static_assert(
            gcd != 0,
            "PowRatio exp numerator num and denominator den must not be zero"
        );

        static_assert(
            std::gcd(num, den) == 1,
            "PowRatio exp numerator num and denominator den must be co-prime"
        );

        inline constexpr long double value() const
        {
            constexpr auto _num = static_cast<long double>(num);
            constexpr auto _den = static_cast<long double>(den);
            return _num / _den;
        }
    };

}   // namespace mstd

#endif   // __MSTD_MATH_RATIONAL_HPP__