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

#include "mstd/linAlg/vector3d.hpp"

#include <mp-units/systems/si.h>
#include <mp-units/systems/si/unit_symbols.h>

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

using namespace mp_units;
using namespace mp_units::si::unit_symbols;
using namespace mstd;

TEST_CASE("Vector3D")
{
    constexpr auto distance = Vector3d{1.0 * km, 2.0 * km, 3.0 * km};
    constexpr auto time     = Vector3d{4.0 * h, 5.0 * h, 6.0 * h};
    constexpr auto speed = Vector3d{0.25 * km / h, 0.4 * km / h, 0.5 * km / h};
    constexpr auto distanceMeter = Vector3d{1000 * m, 2000 * m, 3000 * m};
    constexpr auto area          = Vector3d{1e6 * m2, 4e6 * m2, 9e6 * m2};

    STATIC_REQUIRE(speed == distance / time);
    STATIC_REQUIRE(distance == distanceMeter);
    STATIC_REQUIRE(area == distance * distance);
}