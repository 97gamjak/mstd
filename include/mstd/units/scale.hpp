#ifndef __MSTD_UNITS_SCALE_HPP__
#define __MSTD_UNITS_SCALE_HPP__

#include "dimension.hpp"
#include "ratio.hpp"

namespace mstd
{
    // clang-format off
    template <typename R> struct L { using ratio = R; using kind = void; };
    template <typename R> struct M { using ratio = R; using kind = void; };
    template <typename R> struct T { using ratio = R; using kind = void; };
    template <typename R> struct K { using ratio = R; using kind = void; };
    template <typename R> struct N { using ratio = R; using kind = void; };
    template <typename R> struct I { using ratio = R; using kind = void; };
    template <typename R> struct Cd { using ratio = R; using kind = void; };
    // clang-format on

    using Ang = L<std::ratio<1, 10>>;
    using nm  = L<std::nano>;
    using um  = L<std::micro>;
    using mm  = L<std::milli>;
    using m   = L<std::ratio<1>>;
    using km  = L<std::kilo>;

    using ng = M<std::pico>;
    using ug = M<std::nano>;
    using mg = M<std::micro>;
    using g  = M<std::milli>;
    using kg = M<std::ratio<1>>;

    using fs  = T<std::femto>;
    using ps  = T<std::pico>;
    using ns  = T<std::nano>;
    using us  = T<std::micro>;
    using ms  = T<std::milli>;
    using s   = T<std::ratio<1>>;
    using min = T<std::ratio<60>>;
    using h   = T<std::ratio<3600>>;
    using d   = T<std::ratio<86400>>;
    using yr  = T<std::ratio<31536000>>;
    // clang-format on

}   // namespace mstd

namespace mstd
{
    /**
     * @brief ScalePack class
     *
     * @tparam Ls
     * @tparam Ms
     * @tparam Ts
     * @tparam Ks
     * @tparam Ns
     * @tparam Is
     * @tparam Cds
     * @tparam Kind
     */
    template <
        typename Ls   = L<std::ratio<1>>,
        typename Ms   = M<std::ratio<1>>,
        typename Ts   = T<std::ratio<1>>,
        typename Ks   = K<std::ratio<1>>,
        typename Ns   = N<std::ratio<1>>,
        typename Is   = I<std::ratio<1>>,
        typename Cds  = Cd<std::ratio<1>>,
        typename Kind = void>
    struct ScalePack
    {
        using L    = Ls;
        using M    = Ms;
        using T    = Ts;
        using K    = Ks;
        using N    = Ns;
        using I    = Is;
        using Cd   = Cds;
        using kind = Kind;
    };

    namespace units::details
    {

        /**
         * @brief Implements dimension scaling.
         *
         * @tparam Dim
         * @tparam Pack
         */
        template <typename Dim, typename Pack>
        struct dim_scale;

        /**
         * @brief template specialization for dim_scale
         *
         * @tparam L
         * @tparam M
         * @tparam T
         * @tparam K
         * @tparam N
         * @tparam I
         * @tparam Cd
         * @tparam Pack
         */
        template <
            int L,
            int M,
            int T,
            int K,
            int N,
            int I,
            int Cd,
            typename Pack>
        struct dim_scale<dimension<L, M, T, K, N, I, Cd>, Pack>
        {
            using type = ratio_mul_t<
                ratio_pow_t<typename Pack::L::ratio, L>,
                ratio_pow_t<typename Pack::M::ratio, M>,
                ratio_pow_t<typename Pack::T::ratio, T>,
                ratio_pow_t<typename Pack::K::ratio, K>,
                ratio_pow_t<typename Pack::N::ratio, N>,
                ratio_pow_t<typename Pack::I::ratio, I>,
                ratio_pow_t<typename Pack::Cd::ratio, Cd>>;
        };

        /**
         * @brief type alias for dim_scale
         *
         * @tparam Dim
         * @tparam Pack
         */
        template <typename Dim, typename Pack>
        using dim_scale_t = typename dim_scale<Dim, Pack>::type;

    }   // namespace units::details

}   // namespace mstd

#endif   // __MSTD_UNITS_SCALE_HPP__