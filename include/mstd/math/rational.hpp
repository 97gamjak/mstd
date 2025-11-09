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
#include "ratio.hpp"

namespace mstd
{
    template <intmax_t Num = 0, intmax_t Den = 1>
    struct Rational
    {
        static_assert(Den != 0, "Rational denominator Den must not be zero");

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
            []
            {
                // TODO: implement nice generic solution for static_assert with
                // values
                if constexpr (std::gcd(num, den) != 1)
                {
                    print_error<num, den>{};
                    return false;
                }
                else
                    return true;
            }(),
            "PowRatio exp numerator num and denominator den must be co-prime"
        );

        inline static constexpr long double value()
        {
            constexpr auto _num = static_cast<long double>(num);
            constexpr auto _den = static_cast<long double>(den);
            return _num / _den;
        }

        static constexpr bool is_zero = (num == 0);
    };

    template <intmax_t Num = 1, intmax_t Den = 1>
    using Ratio = Rational<Num, Den>;

    template <intmax_t T>
    using DefaultRational = Rational<>;

    template <RationalType R, RationalType S>
    struct add_type<R, S>
    {
        using type =
            Rational<R::num * S::den + S::num * R::den, R::den * S::den>;
    };

    template <RationalType R, RationalType S>
    struct sub_type<R, S>
    {
        using type =
            Rational<R::num * S::den - S::num * R::den, R::den * S::den>;
    };

    template <RationalType R, RationalType S>
    struct mul_type<R, S>
    {
        static_assert(
            R::den != 0 && S::den != 0,
            "Rational denominator must not be zero"
        );

        using type = Rational<R::num * S::num, R::den * S::den>;
    };

    template <RationalType R, IntegralConstantType I>
    struct mul_type<R, I>
    {
        using type = Rational<R::num * I::value, R::den>;
    };

    template <RationalType R, RationalType S>
    struct div_type<R, S>
    {
        static_assert(
            R::den != 0 && S::num != 0,
            "Rational denominator of R and numerator of S must not be zero"
        );

        using type = Rational<R::num * S::den, R::den * S::num>;
    };

    template <RationalType R>
    using inv_rational_t = Rational<R::den, R::num>;

    template <RationalType R>
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

            // Absolute value of the exp
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

    constexpr intmax_t ipow(intmax_t base, unsigned exp)
    {
        return exp == 0    ? 1
               : exp == 1  ? base
               : (exp & 1) ? base * ipow(base, exp - 1)
                           : ipow(base * base, exp / 2);
    }

    template <RationalType Base = Rational<0>, RationalType Exp = Rational<1>>
    struct RationalPower
    {
        using base                    = Base;
        using exp                     = Exp;
        static constexpr bool is_zero = (base::num == 0);

        static constexpr long double value()
        {
            static_assert(
                exp::den > 0,
                "RationalPower exponent denominator must be positive"
            );

            constexpr intmax_t _num = std::
                conditional_t<(exp::num < 0), inv_rational_t<base>, base>::num;
            constexpr intmax_t _den = std::
                conditional_t<(exp::num < 0), inv_rational_t<base>, base>::den;

            constexpr intmax_t _exp = std::
                conditional_t<(exp::num < 0), rational_neg_t<exp>, exp>::num;

            constexpr intmax_t num  = ipow(_num, _exp);
            constexpr intmax_t den  = ipow(_den, _exp);
            constexpr intmax_t num2 = ipow(num, exp::den);
            constexpr intmax_t den2 = ipow(den, exp::den);
            return static_cast<long double>(num2) /
                   static_cast<long double>(den2);
        }
    };

    template <RationalType Base = Rational<1>, RationalType Exp = Rational<1>>
    using RatioPower = RationalPower<Base, Exp>;

    template <intmax_t T>
    using DefaultRationalPower = RationalPower<>;

    template <RationalPowerType R, RationalPowerType S>
    struct rational_mul_impl
    {
        using type = RationalPower<
            mul_type_t<typename R::base, typename S::base>,
            add_type_t<typename R::exp, typename S::exp>>;
    };

    template <RationalPowerType R, RationalPowerType S>
    struct rational_div_impl
    {
        using type = RationalPower<
            div_type_t<typename R::base, typename S::base>,
            sub_type_t<typename R::exp, typename S::exp>>;
    };

    template <RationalPowerType R, RationalPowerType S>
    struct rational_add_impl
    {
        static_assert(
            R::exp::num == S::exp::num,
            "RationalPower exponents must be equal to add them"
        );

        static_assert(
            R::exp::den == S::exp::den,
            "RationalPower exponents must be equal to add them"
        );

        using type = RationalPower<
            add_type_t<typename R::base, typename S::base>,
            typename R::exp>;
    };

    template <RationalPowerType R, IntegralConstantType I>
    struct pow_type<R, I>
    {
        using type =
            RationalPower<typename R::base, mul_type_t<typename R::exp, I>>;
    };

    // Primary template
    template <RationalType R, intmax_t I>
    struct pow_type_impl;

    // Case 1: exponent == 0  →  R^0 = 1
    template <RationalType R>
    struct pow_type_impl<R, 0>
    {
        using type = Rational<1, 1>;
    };

    // Case 2: exponent > 0   →  R^n = R * R^(n-1)
    template <RationalType R, intmax_t I>
    requires(I > 0)
    struct pow_type_impl<R, I>
    {
        using type = mul_type_t<R, typename pow_type_impl<R, I - 1>::type>;
    };

    // Case 3: exponent < 0   →  R^(-n) = (1/R)^n
    template <RationalType R, intmax_t I>
    requires(I < 0)
    struct pow_type_impl<R, I>
    {
        using type = typename pow_type_impl<inv_rational_t<R>, -I>::type;
    };

    template <RationalType R, IntegralConstantType I>
    struct pow_type<R, I>
    {
        using type = typename pow_type_impl<R, I::value>::type;
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

    template <RationalType R, RationalType S>
    struct gcd
    {
        static constexpr auto _num = std::gcd(R::num, S::num);
        static constexpr auto _den = std::lcm(R::den, S::den);

        static_assert(_den != 0, "Rational denominator must not be zero");

        using type = Rational<_num, _den>;
    };

    template <RationalType R, RationalType S>
    using gcd_t = typename gcd<R, S>::type;

    template <RationalType R, RationalType S>
    struct lcm
    {
        static constexpr auto _num = std::lcm(R::num, S::num);
        static constexpr auto _den = std::gcd(R::den, S::den);

        using type = Rational<_num, _den>;
    };

    template <RationalType R, RationalType S>
    using lcm_t = typename lcm<R, S>::type;

    template <RationalPowerType R, RationalPowerType S>
    struct mul_type<R, S>
    {
        using common_exp = gcd_t<typename R::exp, typename S::exp>;
        using expR       = div_type_t<typename R::exp, common_exp>;
        using expS       = div_type_t<typename S::exp, common_exp>;

        using _R = pow_type_t<typename R::base, IntegralConst<expR::num>>;
        using _S = pow_type_t<typename S::base, IntegralConst<expS::num>>;

        using base = mul_type_t<_R, _S>;

        static_assert(
            common_exp::den != 0,
            "RationalPower exponent denominator must not be zero"
        );

        using type = RationalPower<base, common_exp>;
    };

    template <RationalPowerType R, RationalPowerType S>
    struct div_type<R, S>
    {
        using common_exp = gcd_t<typename R::exp, typename S::exp>;
        using expR       = div_type_t<typename R::exp, common_exp>;
        using expS       = mul_type_t<typename S::exp, common_exp>;

        using _R = pow_type_t<typename R::base, IntegralConst<expR::num>>;
        using _S = pow_type_t<typename S::base, IntegralConst<expS::num>>;

        using base = div_type_t<_R, _S>;

        static_assert(
            common_exp::den != 0,
            "RationalPower exponent denominator must not be zero"
        );

        using type = RationalPower<base, common_exp>;
    };

    template <RationalPowerType R, RationalPowerType S>
    requires(R::exp::num == 0 && S::exp::num == 0)
    struct div_type<R, S>
    {
        // return 1 as a RatioPower
        using type = RatioPower<>;
    };

    template <RationalType R>
    struct ratio_v_impl<R>
    {
        static constexpr long double value = R::value();
    };

    template <RationalPowerType R>
    struct ratio_v_impl<R>
    {
        static constexpr long double value = R::value();
    };

    using femto = RationalPower<Rational<10>, Rational<-15>>;
    using pico  = RationalPower<Rational<10>, Rational<-12>>;
    using nano  = RationalPower<Rational<10>, Rational<-9>>;
    using micro = RationalPower<Rational<10>, Rational<-6>>;
    using milli = RationalPower<Rational<10>, Rational<-3>>;
    using centi = RationalPower<Rational<10>, Rational<-2>>;
    using deci  = RationalPower<Rational<10>, Rational<-1>>;
    using deca  = RationalPower<Rational<10>, Rational<1>>;
    using hecto = RationalPower<Rational<10>, Rational<2>>;
    using kilo  = RationalPower<Rational<10>, Rational<3>>;
    using mega  = RationalPower<Rational<10>, Rational<6>>;
    using giga  = RationalPower<Rational<10>, Rational<9>>;
    using tera  = RationalPower<Rational<10>, Rational<12>>;
    using peta  = RationalPower<Rational<10>, Rational<15>>;

}   // namespace mstd

#endif   // __MSTD_MATH_RATIONAL_HPP__