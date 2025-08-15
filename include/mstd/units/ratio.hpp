#ifndef __MSTD_UNITS_RATIO_HPP__
#define __MSTD_UNITS_RATIO_HPP__

#include <ratio>

namespace mstd
{
    template <typename R, unsigned AbsN>
    struct ratio_pow_core
    {
        using prev = typename ratio_pow_core<R, AbsN - 1u>::type;
        using type = std::ratio_multiply<prev, R>;
    };

    template <typename R>
    struct ratio_pow_core<R, 0u>
    {
        using type = std::ratio<1>;
    };

    template <typename R, int N>
    struct ratio_pow
    {
        static constexpr bool     Neg = N < 0;
        static constexpr unsigned AbsN =
            Neg ? static_cast<unsigned>(-N) : static_cast<unsigned>(N);

        using pos = typename ratio_pow_core<R, AbsN>::type;
        using type =
            std::conditional_t<Neg, std::ratio<pos::den, pos::num>, pos>;
    };

    template <class R, int N>
    using ratio_pow_t = typename ratio_pow<R, N>::type;

    template <typename... Ratios>
    struct ratio_mul;

    template <>
    struct ratio_mul<>
    {
        using type = std::ratio<1>;
    };

    template <typename R1, typename... Ratios>
    struct ratio_mul<R1, Ratios...>
    {
        using type =
            std::ratio_multiply<R1, typename ratio_mul<Ratios...>::type>;
    };

    template <typename... Ratios>
    using ratio_mul_t = typename ratio_mul<Ratios...>::type;

}   // namespace mstd

#endif   // __MSTD_UNITS_RATIO_HPP__