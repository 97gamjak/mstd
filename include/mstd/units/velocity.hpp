#ifndef __MSTD_UNITS_ALIASES_HPP__
#define __MSTD_UNITS_ALIASES_HPP__

#include "quantity.hpp"
#include "scale.hpp"
#include "traits.hpp"

namespace mstd
{
    template <typename... Args>
    struct Velocity_;

    template <typename Pack>
    concept VelPack = units::details::pack_is_v<Pack, pack_velocity_t>;

    template <VelPack Pack>
    struct Velocity_<Pack>
    {
        using type = Quantity<double, D_Velocity, Pack>;
    };

    template <LengthTag LTag, TimeTag TTag>
    struct Velocity_<LTag, TTag>
    {
        using type = Quantity<double, D_Velocity, VelocityPack<LTag, TTag>>;
    };

    template <units::details::ValueType T, VelPack Pack>
    struct Velocity_<T, Pack>
    {
        using type = Quantity<T, D_Velocity, Pack>;
    };

    template <units::details::ValueType T, LengthTag LTag, TimeTag TTag>
    struct Velocity_<T, LTag, TTag>
    {
        using type = Quantity<T, D_Velocity, VelocityPack<LTag, TTag>>;
    };

    template <typename... Args>
    using Velocity = typename Velocity_<Args...>::type;

}   // namespace mstd

#endif   // __MSTD_UNITS_ALIASES_HPP__