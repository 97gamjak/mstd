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

        // Den == 0 cannot happen at this point, but just to be safe!
        static constexpr intmax_t gcd =
            (Num == 0 || Den == 0) ? 1 : std::gcd(Num, Den);

        static constexpr intmax_t num = rawNum / gcd;
        static constexpr intmax_t den = rawDen / gcd;

        static_assert(
            gcd != 0,
            "PowRatio exp numerator num and denominator den must not be zero"
        );

        static_assert(
            std::gcd(num, den) == 1 || num == 0,
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

    template <RationalPowerType R, RationalType S>
    struct pow_type<R, S>
    {
        using type =
            RationalPower<typename R::base, mul_type_t<typename R::exp, S>>;
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

    constexpr bool will_overflow_mul(intmax_t a, intmax_t b)
    {
        if (a == 0 || b == 0)
            return false;
        if (a == -1)
            return b == INTMAX_MIN;
        if (b == -1)
            return a == INTMAX_MIN;
        return (a > 0 && b > 0 && a > INTMAX_MAX / b) ||
               (a < 0 && b < 0 && a < INTMAX_MAX / b) ||
               (a > 0 && b < 0 && b < INTMAX_MIN / a) ||
               (a < 0 && b > 0 && a < INTMAX_MIN / b);
    }

    template <intmax_t A, intmax_t B>
    struct safe_mul
    {
        static constexpr bool overflow = []
        {
            if constexpr (A == 0 || B == 0)
                return false;
            if constexpr (A == -1)
                return B == INTMAX_MIN;
            if constexpr (B == -1)
                return A == INTMAX_MIN;

            // All other cases: check bounds
            if constexpr (A > 0 && B > 0)
                return A > INTMAX_MAX / B;
            if constexpr (A < 0 && B < 0)
                return A < INTMAX_MAX / B;
            if constexpr (A > 0 && B < 0)
                return B < INTMAX_MIN / A;
            if constexpr (A < 0 && B > 0)
                return A < INTMAX_MIN / B;

            return false;
        }();

        static_assert(!overflow, "Rational overflow in RationalPower multiply");

        static constexpr intmax_t value = A * B;
    };

    constexpr intmax_t cabs(intmax_t x) noexcept { return x < 0 ? -x : x; }

    template <class R>
    struct ratio_simplify;

    template <intmax_t N, intmax_t D>
    struct ratio_simplify<Rational<N, D>>
    {
        static_assert(D != 0);
        static constexpr intmax_t sN = (D < 0 ? -N : N);
        static constexpr intmax_t sD = (D < 0 ? -D : D);
        static constexpr intmax_t g =
            std::gcd(sN == 0 ? intmax_t(1) : cabs(sN), sD);
        using type = Rational<sN / g, sD / g>;
    };

    template <class R1, class R2>
    struct ratio_mul_simplified
    {
        // Like std::ratio_multiply but with gcd to shrink intermediates.
        static constexpr intmax_t g1 = std::gcd(cabs(R1::num), R2::den);
        static constexpr intmax_t g2 = std::gcd(cabs(R2::num), R1::den);

        static constexpr intmax_t n1 = R1::num / g1;
        static constexpr intmax_t d1 = R1::den / g2;
        static constexpr intmax_t n2 = R2::num / g2;
        static constexpr intmax_t d2 = R2::den / g1;

        static constexpr intmax_t num = safe_mul<n1, n2>::value;
        static constexpr intmax_t den = safe_mul<d1, d2>::value;

        using type = typename ratio_simplify<Rational<num, den>>::type;
    };

    // Slight tweak for negative E to avoid re-using impl incorrectly:
    template <class R>
    struct invert_ratio
    {
        using type = typename ratio_simplify<Rational<R::den, R::num>>::type;
    };

    // Forward declaration
    template <class R, intmax_t E>
    struct ratio_pow_nonneg;

    // E == 0 -> 1
    template <class R>
    struct ratio_pow_nonneg<R, 0>
    {
        using type = Rational<1, 1>;
    };

    // E == 1 -> R   (crucial: no squaring here)
    template <class R>
    struct ratio_pow_nonneg<R, 1>
    {
        using type = R;
    };

    // E >= 2: exponentiation by squaring
    template <class R, intmax_t E>
    struct ratio_pow_nonneg
    {
        static_assert(E >= 2, "ratio_pow_nonneg requires E >= 0");

        using half    = typename ratio_pow_nonneg<R, (E / 2)>::type;
        using half_sq = typename ratio_mul_simplified<half, half>::type;

        using type = std::conditional_t<
            (E % 2 == 0),
            // even: R^E = (R^(E/2))^2
            half_sq,
            // odd:  R^E = R * (R^((E-1)/2))^2
            typename ratio_mul_simplified<R, half_sq>::type>;
    };

    // Main entry: handles negative exponents by inverting the base.
    template <class R, intmax_t E>
    struct ratio_pow_int
    {
       private:
        static constexpr bool neg = (E < 0);
        using base_pos =
            std::conditional_t<neg, typename invert_ratio<R>::type, R>;
        static constexpr intmax_t exp_pos = neg ? -E : E;

       public:
        using type = typename ratio_pow_nonneg<base_pos, exp_pos>::type;
    };

    template <class R, intmax_t E>
    using ratio_pow_int_fixed = typename ratio_pow_int<R, E>::type;

    template <RationalPowerType R, RationalPowerType S>
    struct mul_type<R, S>
    {
        using B1 = typename R::base;   // Rational<a1,b1>
        using E1 = typename R::exp;    // Rational<n1,d1>
        using B2 = typename S::base;   // Rational<a2,b2>
        using E2 = typename S::exp;    // Rational<n2,d2>

        static constexpr intmax_t n1 = E1::num;
        static constexpr intmax_t d1 = E1::den;
        static constexpr intmax_t n2 = E2::num;
        static constexpr intmax_t d2 = E2::den;

        // Common denominator of the exponents
        static constexpr intmax_t L = std::lcm(d1, d2);

        // Integerized exponents
        static constexpr intmax_t k1 = n1 * (L / d1);
        static constexpr intmax_t k2 = n2 * (L / d2);

        // Handle degenerate case: one exponent zero etc.
        // (You might want extra branches here if desired.)

        // Factor out gcd to keep new exponent small
        static constexpr intmax_t t = std::gcd(cabs(k1), cabs(k2));
        // If both zero, t = 0; treat separately:
        static constexpr bool all_zero = (k1 == 0 && k2 == 0);

        static constexpr intmax_t u1 = all_zero ? 0 : k1 / t;
        static constexpr intmax_t u2 = all_zero ? 0 : k2 / t;

        // New exponent before simplification: t / L
        static constexpr intmax_t gexp  = all_zero ? 1 : std::gcd(cabs(t), L);
        static constexpr intmax_t n_exp = all_zero ? 0 : (t / gexp);
        static constexpr intmax_t d_exp = all_zero ? 1 : (L / gexp);

        // Base = (B1^u1) * (B2^u2)
        using B1u = ratio_pow_int_fixed<B1, u1>;
        using B2u = ratio_pow_int_fixed<B2, u2>;
        using B   = typename ratio_mul_simplified<B1u, B2u>::type;

        using Exp = Rational<n_exp, d_exp>;

        using type = std::conditional_t<
            all_zero,
            // (anything)^0 = 1
            RationalPower<Rational<1, 1>, Rational<0, 1>>,
            RationalPower<B, Exp>>;
    };

    template <RationalPowerType R, RationalPowerType S>
    requires(R::exp::num == 0 && S::exp::num == 0)
    struct mul_type<R, S>
    {
        using type = RatioPower<>;
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

        using type = RationalPower<base, common_exp>;
    };

    template <RationalPowerType R, RationalPowerType S>
    requires(R::exp::num == 0 && S::exp::num == 0)
    struct div_type<R, S>
    {
        using type = RatioPower<>;
    };

    template <RationalPowerType R, RationalPowerType S>
    requires(R::exp::num != 0 && S::exp::num == 0)
    struct div_type<R, S>
    {
        using type = div_type_t<R, RatioPower<>>;
    };

    template <RationalPowerType R, RationalPowerType S>
    requires(R::exp::num == 0 && S::exp::num != 0)
    struct div_type<R, S>
    {
        using type = div_type_t<RatioPower<>, S>;
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