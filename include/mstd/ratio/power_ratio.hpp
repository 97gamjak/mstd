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

#ifndef __MSTD_RATIO_POWER_RATIO_HPP__
#define __MSTD_RATIO_POWER_RATIO_HPP__

#include <cmath>
#include <numeric>
#include <ratio>

#include "mstd/error.hpp"
#include "mstd/math.hpp"
#include "mstd/type_traits/ratio_traits.hpp"

namespace mstd
{
    template <
        intmax_t Num  = 1,
        intmax_t Den  = 1,
        intmax_t powP = 1,
        intmax_t powQ = 1>
    struct PowRatio
    {
        using ratio    = std::ratio<Num, Den>;
        using rational = Rational<powP, powQ>;

        static constexpr auto num = Num;
        static constexpr auto den = Den;
        static constexpr auto p   = rational::num;
        static constexpr auto q   = rational::den;

        inline constexpr auto getExp() const { return rational::value(); }
    };

    template <typename T>
    inline constexpr long double ratio_v = 1.0L;

    template <RatioType T>
    static constexpr long double ratio_v<T> =
        static_cast<long double>(T::num) / static_cast<long double>(T::den);

    template <PowRatioType T>
    static constexpr long double ratio_v<T> =
        std::pow(ratio_v<typename T::ratio>, T::getExp());

    template <typename R1, typename R2>
    struct ratio_mul_impl
    {
        MSTD_COMPILE_FAIL("Unsupported ratio type");
    };

    template <RatioType R1, RatioType R2>
    struct ratio_mul_impl<R1, R2>
    {
        using type = std::ratio_multiply<R1, R2>;
    };

    template <PowRatioType R1, PowRatioType R2>
    struct ratio_mul_impl<R1, R2>
    {
        using type = PowRatio<
            std::ratio_multiply<typename R1::ratio, typename R2::ratio>::num,
            std::ratio_multiply<typename R1::ratio, typename R2::ratio>::den,
            R1::p + R2::p,
            R1::q + R2::q>;
    };

    template <typename R1, typename R2>
    using ratio_mul_t = ratio_mul_impl<R1, R2>::type;

    template <typename R1, typename R2>
    struct ratio_div_impl
    {
        MSTD_COMPILE_FAIL("Unsupported ratio type");
    };

    template <RatioType R1, RatioType R2>
    struct ratio_div_impl<R1, R2>
    {
        using type = std::ratio_divide<R1, R2>;
    };

    template <PowRatioType R1, PowRatioType R2>
    struct ratio_div_impl<R1, R2>
    {
        using type = PowRatio<
            std::ratio_divide<typename R1::ratio, typename R2::ratio>::num,
            std::ratio_divide<typename R1::ratio, typename R2::ratio>::den,
            R1::p - R2::p,
            R1::q - R2::q>;
    };

    template <typename R1, typename R2>
    using ratio_div_t = ratio_div_impl<R1, R2>::type;

    // TODO: cleanup

}   // namespace mstd

#endif   // __MSTD_RATIO_POWER_RATIO_HPP__