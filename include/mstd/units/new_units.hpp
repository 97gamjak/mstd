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

#ifndef __MSTD__UNITS__NEW_UNITS_HPP__
#define __MSTD__UNITS__NEW_UNITS_HPP__

#include <mp-units/framework.h>
#include <mp-units/systems/si/units.h>

#include "mp-units/systems/si/unit_symbols.h"
#include "namespace.hpp"   // IWYU pragma: keep

namespace mstd::units
{
    inline constexpr struct bar final
        : mp_units::named_unit<"bar", mp_units::mag<100'000> * Pa>
    {
    } bar;

    // here starting only aliases

    inline constexpr auto per_cm  = one / cm;
    inline constexpr auto per_bar = one / bar;

}   // namespace mstd::units

#endif   // __MSTD__UNITS__NEW_UNITS_HPP__
