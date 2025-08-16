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