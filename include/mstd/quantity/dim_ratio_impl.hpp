#ifndef __MSTD_DIM_RATIO_IMPL_HPP__
#define __MSTD_DIM_RATIO_IMPL_HPP__

#include "dim_ratio.hpp"
#include "mstd/ratio.hpp"

namespace mstd
{
    template <StdRatio R1 = std::ratio<1>>
    struct length_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_length, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using length_dim_ratio = typename length_dim_ratio_impl<R1>::type;

    template <StdRatio R1 = std::ratio<1>>
    struct mass_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_mass, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using mass_dim_ratio = typename mass_dim_ratio_impl<R1>::type;

    template <StdRatio R1 = std::ratio<1>>
    struct time_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_time, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using time_dim_ratio = typename time_dim_ratio_impl<R1>::type;

    template <StdRatio R1 = std::ratio<1>>
    struct current_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_current, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using current_dim_ratio = typename current_dim_ratio_impl<R1>::type;

    template <StdRatio R1 = std::ratio<1>>
    struct temp_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_temp, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using temp_dim_ratio = typename temp_dim_ratio_impl<R1>::type;

    template <StdRatio R1 = std::ratio<1>>
    struct amount_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_amount, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using amount_dim_ratio = typename amount_dim_ratio_impl<R1>::type;

    template <StdRatio R1 = std::ratio<1>>
    struct luminous_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_luminous, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using luminous_dim_ratio = typename luminous_dim_ratio_impl<R1>::type;

    template <StdRatio R1 = std::ratio<1>>
    struct angle_dim_ratio_impl
    {
        using type = make_dim_ratio_single_t<dim_angle, R1>;
    };

    template <StdRatio R1 = std::ratio<1>>
    using angle_dim_ratio = typename angle_dim_ratio_impl<R1>::type;
}   // namespace mstd

#endif   // __MSTD_DIM_RATIO_IMPL_HPP__