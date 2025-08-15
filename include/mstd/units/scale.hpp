#ifndef __MSTD_UNITS_SCALE_HPP__
#define __MSTD_UNITS_SCALE_HPP__

#include "dimension.hpp"
#include "ratio.hpp"
#include "scale_tags.hpp"

namespace mstd
{
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
        LengthTag            Ls  = L<std::ratio<1>>,
        MassTag              Ms  = M<std::ratio<1>>,
        TimeTag              Ts  = T<std::ratio<1>>,
        TemperatureTag       Ks  = Temp<std::ratio<1>>,
        AmountOfSubstanceTag Ns  = N<std::ratio<1>>,
        CurrentTag           Is  = I<std::ratio<1>>,
        LuminousIntensityTag Cds = Candela<std::ratio<1>>,
        typename Kind            = void>
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

    // clang-format off
    struct pack_length_t{};
    struct pack_mass_t{};
    struct pack_time_t{};
    struct pack_temperature_t{};
    struct pack_amount_of_substance_t{};
    struct pack_current_t{};
    struct pack_luminous_intensity_t{};

    struct pack_velocity_t{};
    struct pack_acceleration_t{};
    // clang-format on

    template <LengthTag LTag>
    using LengthPack = ScalePack<LTag, kg, s, K, mol, A, Cd, pack_length_t>;

    template <TimeTag TTag>
    using TimePack = ScalePack<m, kg, TTag, K, mol, A, Cd, pack_time_t>;

    template <MassTag MTag>
    using MassPack = ScalePack<m, MTag, s, K, mol, A, Cd, pack_mass_t>;

    template <TemperatureTag TTag>
    using TempPack = ScalePack<m, kg, s, TTag, mol, A, Cd, pack_temperature_t>;

    template <AmountOfSubstanceTag NTag>
    using AmountOfSubstancePack =
        ScalePack<m, kg, s, K, NTag, A, Cd, pack_amount_of_substance_t>;

    template <CurrentTag ITag>
    using CurrentPack = ScalePack<m, kg, s, K, mol, ITag, Cd, pack_current_t>;

    template <LuminousIntensityTag CdsTag>
    using LuminousIntensityPack =
        ScalePack<m, kg, s, K, mol, A, CdsTag, pack_luminous_intensity_t>;

    template <LengthTag LTag, TimeTag TTag>
    using VelocityPack =
        ScalePack<LTag, kg, TTag, K, mol, A, Cd, pack_velocity_t>;

}   // namespace mstd

#endif   // __MSTD_UNITS_SCALE_HPP__