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

TEST_CASE("Vector3d mp-units - Parametrized Constructors")
{
    constexpr auto valueConstructed = Vector3d{1 * m, 2 * m, 3 * m};
    STATIC_REQUIRE(valueConstructed == Vector3d{1 * m, 2 * m, 3 * m});

    constexpr auto scalarConstructed = Vector3d{7 * m};
    STATIC_REQUIRE(scalarConstructed == Vector3d{7 * m, 7 * m, 7 * m});
}

TEST_CASE("Vector3d mp-units - Indexing Operators")
{
    constexpr auto distance = Vector3d{1000 * m, 2000 * m, 3000 * m};

    STATIC_REQUIRE(distance[0] == 1 * km);
    STATIC_REQUIRE(distance[1] == 2 * km);
    STATIC_REQUIRE(distance[2] == 3 * km);

    auto position = Vector3d{0 * km};

    position[0] = 1 * km;
    position[1] = 2 * km;
    position[2] = 3 * km;

    REQUIRE(distance == position);
}

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
    constexpr auto distanceDiff  = Vector3d{999.0 * m, 1999.0 * m, 2999.0 * m};

    // clang-format off
    STATIC_REQUIRE(distance + distanceMeter == distanceTwice);
    STATIC_REQUIRE(scalarMeter + distanceMeter == distanceMeter + scalarMeter);
    STATIC_REQUIRE(distance - distanceMeter == Vector3d{0 * m});
    STATIC_REQUIRE(distanceMeter - scalarMeter == distanceDiff);
    // clang-format on
}

TEST_CASE("Vector3d mp-units - Binary * and / Operators")
{
    constexpr auto distance = Vector3d{1.0 * km, 2.0 * km, 4.0 * km};
    constexpr auto time     = Vector3d{2.0 * h, 1.0 * h, 4.0 * h};
    constexpr auto speed = Vector3d{0.5 * km / h, 2.0 * km / h, 1.0 * km / h};
    constexpr auto area  = Vector3d{1e6 * m2, 4e6 * m2, 16e6 * m2};

    STATIC_REQUIRE(area == distance * distance);
    STATIC_REQUIRE(2 * distance == distance * 2);
    STATIC_REQUIRE(speed == distance / time);
    STATIC_REQUIRE(distance / 2 == distance * 0.5);
}

TEST_CASE("Vector3d mp-units - Norm Functions")
{
    constexpr auto time = Vector3d{2.0 * h, 3.0 * h, 6.0 * h};

    STATIC_REQUIRE(normSquared(time) == (49.0 * h * h));
    REQUIRE(norm(time) == (7.0 * h));
}

TEST_CASE("Vector3d mp-units - Nested Vector3d Indexing Operator")
{
    constexpr auto pos1 = Vector3d{1000 * m, 2000 * m, 3000 * m};
    constexpr auto pos2 = Vector3d{4000 * m, 5000 * m, 6000 * m};
    constexpr auto pos3 = Vector3d{7000 * m, 8000 * m, 9000 * m};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    STATIC_REQUIRE(pos[0] == pos1);
    STATIC_REQUIRE(pos[1] == pos2);
    STATIC_REQUIRE(pos[2] == pos3);

    STATIC_REQUIRE(pos[0][0] == 1 * km);
    STATIC_REQUIRE(pos[0][1] == 2 * km);
    STATIC_REQUIRE(pos[0][2] == 3 * km);

    STATIC_REQUIRE(pos[1][0] == 4 * km);
    STATIC_REQUIRE(pos[1][1] == 5 * km);
    STATIC_REQUIRE(pos[1][2] == 6 * km);

    STATIC_REQUIRE(pos[2][0] == 7 * km);
    STATIC_REQUIRE(pos[2][1] == 8 * km);
    STATIC_REQUIRE(pos[2][2] == 9 * km);
}

TEST_CASE("Vector3d mp-units - Nested Vector3d Unary +/-")
{
    constexpr auto pos1        = Vector3d{1 * m, 2 * m, 3 * m};
    constexpr auto pos2        = Vector3d{4 * m, 5 * m, 6 * m};
    constexpr auto pos3        = Vector3d{7 * m, 8 * m, 9 * m};
    constexpr auto pos         = Vector3d{pos1, pos2, pos3};
    constexpr auto negativePos = Vector3d{-pos1, -pos2, -pos3};

    STATIC_REQUIRE(+pos == pos);
    STATIC_REQUIRE(-pos == negativePos);
}

TEST_CASE("Vector3d mp-units - Nested Vector3d Binary +")
{
    constexpr auto pos1 = Vector3d{1 * m, 2 * m, 3 * m};
    constexpr auto pos2 = Vector3d{4 * m, 5 * m, 6 * m};
    constexpr auto pos3 = Vector3d{7 * m, 8 * m, 9 * m};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    constexpr auto shift1 = Vector3d{10 * m, 11 * m, 12 * m};
    constexpr auto shift2 = Vector3d{13 * m, 14 * m, 15 * m};
    constexpr auto shift3 = Vector3d{16 * m, 17 * m, 18 * m};
    constexpr auto shift  = Vector3d{shift1, shift2, shift3};

    constexpr auto pos1_shifted = Vector3d{11 * m, 13 * m, 15 * m};
    constexpr auto pos2_shifted = Vector3d{17 * m, 19 * m, 21 * m};
    constexpr auto pos3_shifted = Vector3d{23 * m, 25 * m, 27 * m};
    constexpr auto pos_shifted =
        Vector3d{pos1_shifted, pos2_shifted, pos3_shifted};

    STATIC_REQUIRE(pos + shift == pos_shifted);
    STATIC_REQUIRE(shift + pos == pos_shifted);
}

TEST_CASE("Vector3d mp-units - Nested Vector3d Binary -")
{
    constexpr auto pos1 = Vector3d{10 * m, 11 * m, 12 * m};
    constexpr auto pos2 = Vector3d{13 * m, 14 * m, 15 * m};
    constexpr auto pos3 = Vector3d{16 * m, 17 * m, 18 * m};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    constexpr auto shift1 = Vector3d{9 * m, 8 * m, 7 * m};
    constexpr auto shift2 = Vector3d{6 * m, 5 * m, 4 * m};
    constexpr auto shift3 = Vector3d{3 * m, 2 * m, 1 * m};
    constexpr auto shift  = Vector3d{shift1, shift2, shift3};

    constexpr auto pos1_shifted = Vector3d{1 * m, 3 * m, 5 * m};
    constexpr auto pos2_shifted = Vector3d{7 * m, 9 * m, 11 * m};
    constexpr auto pos3_shifted = Vector3d{13 * m, 15 * m, 17 * m};
    constexpr auto pos_shifted =
        Vector3d{pos1_shifted, pos2_shifted, pos3_shifted};

    STATIC_REQUIRE(pos - shift == pos_shifted);
}

TEST_CASE("Vector3d mp-units - Nested Vector3d Binary *")
{
    constexpr auto pos1 = Vector3d{1 * m, 2 * m, 3 * m};
    constexpr auto pos2 = Vector3d{4 * m, 5 * m, 6 * m};
    constexpr auto pos3 = Vector3d{7 * m, 8 * m, 9 * m};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    constexpr auto factor1 = Vector3d{10 * m, 11 * m, 12 * m};
    constexpr auto factor2 = Vector3d{13 * m, 14 * m, 15 * m};
    constexpr auto factor3 = Vector3d{16 * m, 17 * m, 18 * m};
    constexpr auto factor  = Vector3d{factor1, factor2, factor3};

    constexpr auto pos1_scaled = Vector3d{10 * m2, 22 * m2, 36 * m2};
    constexpr auto pos2_scaled = Vector3d{52 * m2, 70 * m2, 90 * m2};
    constexpr auto pos3_scaled = Vector3d{112 * m2, 136 * m2, 162 * m2};
    constexpr auto pos_scaled = Vector3d{pos1_scaled, pos2_scaled, pos3_scaled};

    STATIC_REQUIRE(pos * factor == pos_scaled);
    STATIC_REQUIRE(factor * pos == pos_scaled);
}

TEST_CASE("Vector3d mp-units - Nested Vector3d Binary /")
{
    constexpr auto num1 = Vector3d{1 * m2, 4 * m2, 9 * m2};
    constexpr auto num2 = Vector3d{16 * m2, 25 * m2, 36 * m2};
    constexpr auto num3 = Vector3d{49 * m2, 64 * m2, 81 * m2};
    constexpr auto num  = Vector3d{num1, num2, num3};

    constexpr auto den1 = Vector3d{1 * m, 2 * m, 3 * m};
    constexpr auto den2 = Vector3d{4 * m, 5 * m, 6 * m};
    constexpr auto den3 = Vector3d{7 * m, 8 * m, 9 * m};
    constexpr auto den  = Vector3d{den1, den2, den3};

    constexpr auto quotient1 = Vector3d{1 * m, 2 * m, 3 * m};
    constexpr auto quotient2 = Vector3d{4 * m, 5 * m, 6 * m};
    constexpr auto quotient3 = Vector3d{7 * m, 8 * m, 9 * m};
    constexpr auto quotient  = Vector3d{quotient1, quotient2, quotient3};

    STATIC_REQUIRE(num / den == quotient);
}