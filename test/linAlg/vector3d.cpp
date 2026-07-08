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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

using namespace mstd;

TEST_CASE("Vector3d - Comparison Operators")
{
    constexpr auto distance        = Vector3d{1.0, 2.0, 3.0};
    constexpr auto distanceInt     = Vector3d{1, 2, 3};
    constexpr auto anotherDistance = Vector3d{1.1, 2.2, 3.3};

    STATIC_REQUIRE(distance == distance);
    STATIC_REQUIRE(distance == distanceInt);
    STATIC_REQUIRE(distance != anotherDistance);
}

TEST_CASE("Vector3d - Unary +/- Operators")
{
    constexpr auto distance         = Vector3d{1.0, 2.0, 3.0};
    constexpr auto negativeDistance = Vector3d{-1.0, -2.0, -3.0};

    STATIC_REQUIRE(+distance == distance);
    STATIC_REQUIRE(-distance == negativeDistance);
}

TEST_CASE("Vector3d - Binary + and - Operators")
{
    constexpr auto distance      = Vector3d{1.0, 2.0, 3.0};
    constexpr auto distanceMeter = Vector3d{1000.0, 2000.0, 3000.0};
    constexpr auto distanceTwice = Vector3d{2.0, 4.0, 6.0};
    constexpr auto scalarMeter   = 1.0;

    // clang-format off
    STATIC_REQUIRE(distance + distance == distanceTwice);
    STATIC_REQUIRE(scalarMeter + distanceMeter == distanceMeter + scalarMeter);
    STATIC_REQUIRE(distance - distance == Vector3d{0});
    STATIC_REQUIRE(distanceMeter - scalarMeter == -(scalarMeter - distanceMeter));
    // clang-format on
}

TEST_CASE("Vector3d - Binary * and / Operators")
{
    constexpr auto distance    = Vector3d{1.0, 2.0, 4.0};
    constexpr auto time        = Vector3d{2.0, 1.0, 4.0};
    constexpr auto speed       = Vector3d{0.5, 2.0, 1.0};
    constexpr auto area        = Vector3d{1.0, 4.0, 16.0};
    constexpr auto invDistance = Vector3d{1.0, 0.5, 0.25};

    STATIC_REQUIRE(area == distance * distance);
    STATIC_REQUIRE(2 * distance == distance * 2);
    STATIC_REQUIRE(speed == distance / time);
    STATIC_REQUIRE(distance / 2 == distance * 0.5);
    STATIC_REQUIRE(1 / distance == invDistance);
}

TEST_CASE("Vector3d - Norm Functions")
{
    constexpr auto time = Vector3d{2.0, 3.0, 6.0};

    STATIC_REQUIRE(normSquared(time) == (49.0));
    REQUIRE(norm(time) == (7.0));
}