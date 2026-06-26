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

#include <mp-units/systems/si.h>
#include <mp-units/systems/si/unit_symbols.h>

#include <iostream>

using namespace mp_units;
using namespace mp_units::si::unit_symbols;

void mpUnitsExample()
{
    constexpr auto distance = 220.0 * km;
    constexpr auto time     = 2.0 * h;

    const auto speed     = distance / time;
    const auto speed_m_s = speed.force_in(si::metre / si::second);

    std::cout << "distance: " << distance << "\n"
              << "time: " << time << "\n"
              << "speed: " << speed_m_s << "\n";
}