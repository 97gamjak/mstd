#ifndef __MSTD_UNITS_RATIO_HPP__
#define __MSTD_UNITS_RATIO_HPP__

#include <ratio>

namespace mstd
{
    namespace units::details
    {
        /**
         * @brief ratio_pow core struct for impl details
         *
         * @tparam R
         * @tparam AbsN
         */
        template <typename R, unsigned AbsN>
        struct ratio_pow_core
        {
            using prev = typename ratio_pow_core<R, AbsN - 1u>::type;
            using type = std::ratio_multiply<prev, R>;
        };

        /**
         * @brief specializations for ratio_pow core struct
         *
         * @tparam R
         */
        template <typename R>
        struct ratio_pow_core<R, 0u>
        {
            using type = std::ratio<1>;
        };

        /**
         * @brief ratio_pow struct for impl details
         *
         * @tparam R
         * @tparam N
         */
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

        /**
         * @brief type alias for ratio_pow
         *
         * @tparam R
         * @tparam N
         */
        template <class R, int N>
        using ratio_pow_t = typename ratio_pow<R, N>::type;

        /**
         * @brief struct for impl details of ratio_mul
         *
         * @tparam Ratios...
         */
        template <typename... Ratios>
        struct ratio_mul;

        /**
         * @brief specializations for ratio_mul
         *
         */
        template <>
        struct ratio_mul<>
        {
            using type = std::ratio<1>;
        };

        /**
         * @brief template specialization for ratio_mul
         *
         * @tparam R1
         * @tparam Ratios...
         */
        template <typename R1, typename... Ratios>
        struct ratio_mul<R1, Ratios...>
        {
            using type =
                std::ratio_multiply<R1, typename ratio_mul<Ratios...>::type>;
        };

        /**
         * @brief type alias for ratio_mul
         *
         * @tparam Ratios...
         */
        template <typename... Ratios>
        using ratio_mul_t = typename ratio_mul<Ratios...>::type;

    }   // namespace units::details

}   // namespace mstd

#endif   // __MSTD_UNITS_RATIO_HPP__