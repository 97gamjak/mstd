#ifndef __MSTD_RATIO_HPP__
#define __MSTD_RATIO_HPP__

#include "dimension.hpp"
#include "enums.hpp"
#include "mstd/pack.hpp"

namespace mstd::units
{
    using default_si_ratio_pack =
        pack::make_default_ratio_pack_t<SIDimIdMeta::size>;

    using default_extra_ratio_pack =
        pack::make_default_ratio_pack_t<ExtraDimIdMeta::size>;

    template <
        pack::RatioPack SiRatioPack    = default_si_ratio_pack,
        pack::RatioPack ExtraRatioPack = default_extra_ratio_pack>
    struct dim_ratio
    {
        using si = SiRatioPack;
        using ex = ExtraRatioPack;

        template <SIDimId ID>
        using si_ratio =
            typename SiRatioPack::template type_at<static_cast<size_t>(ID)>;

        template <ExtraDimId ID>
        using ex_ratio =
            typename ExtraRatioPack::template type_at<static_cast<size_t>(ID)>;
    };

    template <class R1, class R2>
    using dim_ratio_mul_t = dim_ratio<
        pack::ratio_pack_mul_t<typename R1::si, typename R2::si>,
        pack::ratio_pack_mul_t<typename R1::ex, typename R2::ex>>;

    template <class R1, class R2>
    using dim_ratio_div_t = dim_ratio<
        pack::ratio_pack_div_t<typename R1::si, typename R2::si>,
        pack::ratio_pack_div_t<typename R1::ex, typename R2::ex>>;

    template <class R, int K>
    using dim_ratio_pow_t = dim_ratio<
        pack::ratio_pack_pow_t<typename R::si, K>,
        pack::ratio_pack_pow_t<typename R::ex, K>>;

    namespace details
    {
        template <SimpleBaseDim D, class R>
        struct make_dim_ratio_single
        {
            using siRatio =
                std::conditional_t<has_si_dim<D>(), R, std::ratio<1>>;
            using exRatio =
                std::conditional_t<has_ex_dim<D>(), R, std::ratio<1>>;

            static constexpr auto siIndex = D::si::non_zero_index();
            static constexpr auto exIndex = D::ex::non_zero_index();

            using si = std::conditional_t<
                (siIndex < D::si::size),
                pack::make_ratio_pack_single_t<siRatio, siIndex, D::si::size>,
                pack::make_default_ratio_pack_t<D::si::size>>;

            using ex = std::conditional_t<
                (exIndex < D::ex::size),
                pack::make_ratio_pack_single_t<exRatio, exIndex, D::ex::size>,
                pack::make_default_ratio_pack_t<D::ex::size>>;

            using type = dim_ratio<si, ex>;
        };

    }   // namespace details

    template <class D, class R>
    using make_dim_ratio_single_t =
        typename details::make_dim_ratio_single<D, R>::type;

    template <class>
    struct is_dim_ratio : std::false_type
    {
    };
    template <class Si, class Ex>
    struct is_dim_ratio<dim_ratio<Si, Ex>> : std::true_type
    {
    };

    template <class T>
    inline constexpr bool is_dim_ratio_v = is_dim_ratio<T>::value;

}   // namespace mstd::units

#endif   // __MSTD_RATIO_HPP__