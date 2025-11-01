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

#include <cmath>
#include <numeric>
#include <ratio>

#include "mstd/type_traits.hpp"

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

        inline static constexpr long double value()
        {
            constexpr auto _num = static_cast<long double>(num);
            constexpr auto _den = static_cast<long double>(den);
            return _num / _den;
        }
    };

    template <intmax_t T>
    using DefaultRational = Rational<>;

    template <RationalType R, RationalType S>
    struct add_type<R, S>
    {
        using type =
            Rational<R::num * S::den + S::num * R::den, R::den * S::den>;
    };

    template <RationalType R, RationalType S>
    using rational_sub_t =
        Rational<R::num * S::den - S::num * R::den, R::den * S::den>;

    template <RationalType R, RationalType S>
    struct mul_type<R, S>
    {
        using type = Rational<R::num * S::num, R::den * S::den>;
    };

    template <RationalType R, IntegralConstantType I>
    struct mul_type<R, I>
    {
        using type = Rational<R::num * I::value(), R::den>;
    };

    template <RationalType R, RationalType S>
    struct div_type<R, S>
    {
        using type = Rational<R::num * S::den, R::den * S::num>;
    };

    template <RationalType R>
    using inv_rational_t = Rational<R::den, R::num>;

    template <RationalType R, int K>
    using rational_neg_t = Rational<-R::num, R::den>;

    namespace details
    {
        template <class R, int K>
        struct rational_pow_impl
        {
            MSTD_COMPILE_FAIL("Unsupported ratio type");
        };

        template <RationalType R, int K>
        struct rational_pow_impl<R, K>
        {
            // Handle negative exponents by flipping numerator/denominator
            using _invRatio = inv_rational_t<R>;
            using base      = std::conditional_t<(K < 0), _invRatio, R>;

            // Absolute value of the exponent
            static constexpr int e = (K < 0 ? -K : K);

            // Start from Rational<1,1> and multiply up
            using type = std::conditional_t<
                e == 0,
                Rational<1, 1>,
                mul_type_t<
                    base,
                    typename rational_pow_impl<base, e - 1>::type>>;
        };

        // --- Base cases ---
        template <RationalType R>
        struct rational_pow_impl<R, 0>
        {
            using type = Rational<>;
        };

        template <RationalType R>
        struct rational_pow_impl<R, 1>
        {
            using type = R;
        };

    }   // namespace details

    template <RationalType R, int K>
    using rational_pow_t = typename details::rational_pow_impl<R, K>::type;

    template <
        RationalType Base = Rational<1, 1>,
        RationalType Exp  = Rational<1, 1>>
    struct RationalPower
    {
        using base     = Base;
        using exponent = Exp;

        static constexpr long double value()
        {
            return std::pow(base::value(), exponent::value());
        }
    };

    template <intmax_t T>
    using DefaultRationalPower = RationalPower<>;

    template <RationalPowerType R, RationalPowerType S>
    struct rational_mul_impl
    {
        using type = RationalPower<
            mul_type_t<typename R::base, typename S::base>,
            add_type_t<typename R::exponent, typename S::exponent>>;
    };

    template <RationalPowerType R, RationalPowerType S>
    struct rational_div_impl
    {
        using type = RationalPower<
            div_type_t<typename R::base, typename S::base>,
            sub_type_t<typename R::exponent, typename S::exponent>>;
    };

    template <RationalPowerType R, RationalPowerType S>
    struct rational_add_impl
    {
        static_assert(
            R::exponent::num == S::exponent::num,
            "RationalPower exponents must be equal to add them"
        );

        static_assert(
            R::exponent::den == S::exponent::den,
            "RationalPower exponents must be equal to add them"
        );

        using type = RationalPower<
            add_type_t<typename R::base, typename S::base>,
            typename R::exponent>;
    };

    template <RationalPowerType R, IntegralConstantType I>
    struct pow_type<R, I>
    {
        using type = RationalPower<
            typename R::base,
            mul_type_t<typename R::exponent, I>>;
    };

    template <typename T>
    struct to_pow_rational
    {
        MSTD_COMPILE_FAIL("Unsupported rational type");
    };

    template <RationalType R>
    struct to_pow_rational<R>
    {
        using type = RationalPower<R, Rational<1, 1>>;
    };

    template <RationalPowerType R>
    struct to_pow_rational<R>
    {
        using type = R;
    };

    template <typename T>
    using to_pow_rational_t = typename to_pow_rational<T>::type;

}   // namespace mstd

#endif   // __MSTD_MATH_RATIONAL_HPP__