#ifndef __MSTD_RATIO_POWER_RATIO_HPP__
#define __MSTD_RATIO_POWER_RATIO_HPP__

#include <cmath>
#include <numeric>
#include <ratio>

#include "mstd/error.hpp"
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
        static constexpr intmax_t num = Num;
        static constexpr intmax_t den = Den;

        using ratio = std::ratio<Num, Den>;

        static constexpr intmax_t _gcd =
            powP == 0 || powQ == 0 ? 1 : std::gcd(powP, powQ);

        static constexpr intmax_t p = powP / _gcd;
        static constexpr intmax_t q = powQ / _gcd;

        static_assert(
            _gcd != 0,
            "PowRatio exp numerator p and denominator q must not be zero"
        );

        static_assert(
            std::gcd(p, q) == 1,
            "PowRatio exp numerator p and denominator q must be co-prime"
        );

        inline constexpr long double getExp() const
        {
            return static_cast<long double>(p) / static_cast<long double>(q);
        }
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

}   // namespace mstd

#endif   // __MSTD_RATIO_POWER_RATIO_HPP__