#ifndef __MSTD_UNITS_ALIASES_HPP__
#define __MSTD_UNITS_ALIASES_HPP__

#include "dimension_aliases.hpp"
#include "quantity.hpp"
#include "scale.hpp"
#include "traits.hpp"

namespace mstd
{
    template <LengthTag LTag, TimeTag TTag>
    using VelPack = ScalePack<
        LTag,
        M<std::ratio<1>>,
        TTag,
        K<std::ratio<1>>,
        N<std::ratio<1>>,
        I<std::ratio<1>>,
        Cd<std::ratio<1>>>;

    template <typename... Args>
    struct Velocity_;

    template <VelocityPack Pack>
    struct Velocity_<Pack>
    {
        using type = Quantity<double, D_Velocity, Pack>;
    };

    template <LengthTag LTag, TimeTag TTag>
    struct Velocity_<LTag, TTag>
    {
        using type = Quantity<double, D_Velocity, VelPack<LTag, TTag>>;
    };

    template <ValueType T, VelocityPack Pack>
    struct Velocity_<T, Pack>
    {
        using type = Quantity<T, D_Velocity, Pack>;
    };

    template <ValueType T, LengthTag LTag, TimeTag TTag>
    struct Velocity_<T, LTag, TTag>
    {
        using type = Quantity<T, D_Velocity, VelPack<LTag, TTag>>;
    };

    template <typename... Args>
    using Velocity = typename Velocity_<Args...>::type;

}   // namespace mstd

#endif   // __MSTD_UNITS_ALIASES_HPP__