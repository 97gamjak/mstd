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

using namespace mp_units;
using namespace mp_units::si::unit_symbols;
using namespace mstd;

TEST_CASE("Vector3d")
{
    constexpr auto distance = Vector3d{1.0 * km, 2.0 * km, 3.0 * km};
    constexpr auto time     = Vector3d{2.0 * h, 1.0 * h, 2.0 * h};
    constexpr auto speed = Vector3d{0.5 * km / h, 2.0 * km / h, 1.5 * km / h};
    constexpr auto distanceMeter = Vector3d{1000.0 * m, 2000.0 * m, 3000.0 * m};
    constexpr auto area          = Vector3d{1e6 * m2, 4e6 * m2, 9e6 * m2};
    constexpr auto scalarMeter   = 1.0 * m;
    constexpr auto distanceInt   = Vector3d{1 * m, 2 * m, 3 * m};
    constexpr auto dimlessVector = Vector3d{1.0, 2.0, 2.0};

    // clang-format off
    STATIC_REQUIRE(distance == distanceMeter);
    STATIC_REQUIRE(distance.in(m) == distanceMeter);
    STATIC_REQUIRE(distanceInt.force_in(km) == Vector3d{0 * m});
    STATIC_REQUIRE(distance.numerical_value_in(m) == Vector3d{1000.0, 2000.0, 3000.0});
    STATIC_REQUIRE(distanceInt.force_numerical_value_in(km) == Vector3d{0.0});
    STATIC_REQUIRE(+distance == distance);
    STATIC_REQUIRE(-distance == -1.0 * distance);
    STATIC_REQUIRE(distance != 2 * distance);
    STATIC_REQUIRE(area == distance * distance);
    STATIC_REQUIRE(2 * distance == distance * 2);
    STATIC_REQUIRE(speed == distance / time);
    STATIC_REQUIRE(distance / 2 == distance * 0.5);
    STATIC_REQUIRE(1 / distance == Vector3d{1.0 * (1/km), 0.5 * (1/km), (1.0 / 3.0)  * (1/km)});
    STATIC_REQUIRE(distance + distanceMeter == 2 * distance);
    STATIC_REQUIRE(scalarMeter + distanceMeter == distanceMeter + scalarMeter);
    STATIC_REQUIRE(distance - distanceMeter == Vector3d{0 * m});
    STATIC_REQUIRE(distanceMeter - scalarMeter == -(scalarMeter - distanceMeter));
    STATIC_REQUIRE(normSquared(dimlessVector) == 9.0);
    STATIC_REQUIRE(normSquared(time) == (9.0 * h * h));
    REQUIRE(norm(dimlessVector) == 3.0);
    REQUIRE(norm(time) == (3.0 * h));
    // clang-format on
}