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

#ifndef __MSTD__UNITS_HPP__
#define __MSTD__UNITS_HPP__

#include <mp-units/systems/si.h>
#include <mp-units/systems/si/math.h>   // adjust to where your grep found sin/cos

#include "mstd/math.hpp"   // needed for mstd::detail::adl_tag

namespace mstd::detail
{
    // quantity overloads: more specialized than the generic fallback, so they
    // win
#define MSTD_UNITS_TRIG_HOOK(fn)                                             \
    template <auto R, typename Rep>                                          \
    constexpr auto fn##_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag) \
    {                                                                        \
        return ::mp_units::si::fn(q).numerical_value_in(::mp_units::one);    \
    }

    MSTD_UNITS_TRIG_HOOK(sin)
    MSTD_UNITS_TRIG_HOOK(cos)
    MSTD_UNITS_TRIG_HOOK(tan)
#undef MSTD_UNITS_TRIG_HOOK
}   // namespace mstd::detail

namespace mstd::units
{
    using namespace mp_units;
    using namespace mp_units::isq;
    using namespace mp_units::si::unit_symbols;
}   // namespace mstd::units

namespace mstd
{
    using mp_units::quantity;
}   // namespace mstd

#endif   // __MSTD__UNITS_HPP__
