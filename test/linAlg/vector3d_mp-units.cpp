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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include "mstd/linAlg/vector3d.hpp"

using namespace mp_units;
using namespace mp_units::si::unit_symbols;
using namespace mstd;

TEST_CASE("Vector3d mp-units - Comparison Operators")
{
    constexpr auto distance      = Vector3d{1.0 * km, 2.0 * km, 3.0 * km};
    constexpr auto distanceMeter = Vector3d{1000.0 * m, 2000.0 * m, 3000.0 * m};
    constexpr auto anotherDistance = Vector3d{1.1 * km, 2.2 * km, 3.3 * km};

    STATIC_REQUIRE(distance == distanceMeter);
    STATIC_REQUIRE(distance != anotherDistance);
}

TEST_CASE("Vector3d mp-units Specific Functions")
{
    constexpr auto distance      = Vector3d{1.0 * km, 2.0 * km, 3.0 * km};
    constexpr auto distanceMeter = Vector3d{1000.0 * m, 2000.0 * m, 3000.0 * m};
    constexpr auto distanceMeterInt = Vector3d{1 * m, 2 * m, 3 * m};

    // clang-format off
    STATIC_REQUIRE(distance.in(m) == distanceMeter);
    STATIC_REQUIRE(distanceMeterInt.force_in(km) == Vector3d{0 * m});
    STATIC_REQUIRE(distance.numerical_value_in(m) == Vector3d{1000.0, 2000.0, 3000.0});
    STATIC_REQUIRE(distanceMeterInt.force_numerical_value_in(km) == Vector3d{0.0});
    // clang-format on
}

TEST_CASE("Vector3d mp-units - Unary +/- Operators")
{
    constexpr auto distance         = Vector3d{1.0 * km, 2.0 * km, 3.0 * km};
    constexpr auto negativeDistance = Vector3d{-1.0 * km, -2.0 * km, -3.0 * km};

    STATIC_REQUIRE(+distance == distance);
    STATIC_REQUIRE(-distance == negativeDistance);
}

TEST_CASE("Vector3d mp-units - Binary + and - Operators")
{
    constexpr auto distance      = Vector3d{1.0 * km, 2.0 * km, 3.0 * km};
    constexpr auto distanceMeter = Vector3d{1000.0 * m, 2000.0 * m, 3000.0 * m};
    constexpr auto distanceTwice = Vector3d{2.0 * km, 4.0 * km, 6.0 * km};
    constexpr auto scalarMeter   = 1.0 * m;

    // clang-format off
    STATIC_REQUIRE(distance + distanceMeter == distanceTwice);
    STATIC_REQUIRE(scalarMeter + distanceMeter == distanceMeter + scalarMeter);
    STATIC_REQUIRE(distance - distanceMeter == Vector3d{0 * m});
    STATIC_REQUIRE(distanceMeter - scalarMeter == -(scalarMeter - distanceMeter));
    // clang-format on
}

TEST_CASE("Vector3d mp-units - Binary * and / Operators")
{
    // clang-format off
    constexpr auto distance = Vector3d{1.0 * km, 2.0 * km, 4.0 * km};
    constexpr auto time     = Vector3d{2.0 * h, 1.0 * h, 4.0 * h};
    constexpr auto speed = Vector3d{0.5 * km / h, 2.0 * km / h, 1.0 * km / h};
    constexpr auto area  = Vector3d{1e6 * m2, 4e6 * m2, 16e6 * m2};
    constexpr auto invDistance = Vector3d{1.0 * (1/km), 0.5 * (1/km), 0.25 * (1/km)};
    // clang-format on

    STATIC_REQUIRE(area == distance * distance);
    STATIC_REQUIRE(2 * distance == distance * 2);
    STATIC_REQUIRE(speed == distance / time);
    STATIC_REQUIRE(distance / 2 == distance * 0.5);
    STATIC_REQUIRE(1 / distance == invDistance);
}

TEST_CASE("Vector3d mp-units - Norm Functions")
{
    constexpr auto time = Vector3d{2.0 * h, 3.0 * h, 6.0 * h};

    STATIC_REQUIRE(normSquared(time) == (49.0 * h * h));
    REQUIRE(norm(time) == (7.0 * h));
}