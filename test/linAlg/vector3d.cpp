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
#include <sstream>
#include <utility>

using namespace mstd;

TEST_CASE("Vector3d - Default Constructor")
{
    constexpr Vector3d<int> defaultConstructed{};

    STATIC_REQUIRE(defaultConstructed == Vector3d<int>{0, 0, 0});
}

TEST_CASE("Vector3d - Copy Constructors")
{
    constexpr Vector3d<int> copySource{1, 2, 3};
    constexpr auto          copyConstructed{copySource};
    constexpr auto          copyAssigned = copyConstructed;

    STATIC_REQUIRE(copyConstructed == copySource);
    STATIC_REQUIRE(copyAssigned == copySource);
}

TEST_CASE("Vector3d - Move Constructors")
{
    constexpr Vector3d<int> moveSource{4, 5, 6};
    constexpr auto          moveConstructed{std::move(moveSource)};
    constexpr auto          moveAssigned = std::move(moveConstructed);

    STATIC_REQUIRE(moveConstructed == moveSource);
    STATIC_REQUIRE(moveAssigned == moveSource);
}

TEST_CASE("Vector3d - Parametrized Constructors")
{
    constexpr auto valueConstructed = Vector3d<int>{1, 2, 3};
    STATIC_REQUIRE(valueConstructed == Vector3d<int>{1, 2, 3});

    constexpr auto scalarConstructed = Vector3d<int>{7};
    STATIC_REQUIRE(scalarConstructed == Vector3d<int>{7, 7, 7});
}

TEST_CASE("Vector3d - Indexing Operators")
{
    constexpr auto distance = Vector3d{1.0, 2.0, 3.0};

    STATIC_REQUIRE(distance[0] == 1.0);
    STATIC_REQUIRE(distance[1] == 2.0);
    STATIC_REQUIRE(distance[2] == 3.0);

    auto position = Vector3d{0.0};

    position[0] = 1.0;
    position[1] = 2.0;
    position[2] = 3.0;

    REQUIRE(distance == position);
}

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

TEST_CASE("Vector3d - Compound Assignment Operators")
{
    auto           distance        = Vector3d{1.0, 2.0, 3.0};
    constexpr auto anotherDistance = Vector3d{5.0, 5.0, 5.0};
    constexpr auto scalarDistance  = 5.0;
    constexpr auto distanceSum     = Vector3d{6.0, 7.0, 8.0};
    constexpr auto distanceProduct = Vector3d{5.0, 10.0, 15.0};

    REQUIRE((distance += anotherDistance) == distanceSum);
    REQUIRE((distance -= anotherDistance) == distance);
    REQUIRE((distance += scalarDistance) == distanceSum);
    REQUIRE((distance -= scalarDistance) == distance);

    REQUIRE((distance *= anotherDistance) == distanceProduct);
    REQUIRE((distance /= anotherDistance) == distance);
    REQUIRE((distance *= scalarDistance) == distanceProduct);
    REQUIRE((distance /= scalarDistance) == distance);
}

TEST_CASE("Vector3d - Binary + and - Operators")
{
    constexpr auto distance      = Vector3d{1.0, 2.0, 3.0};
    constexpr auto distanceMeter = Vector3d{1000.0, 2000.0, 3000.0};
    constexpr auto distanceTwice = Vector3d{2.0, 4.0, 6.0};
    constexpr auto distanceDiff  = Vector3d{999.0, 1999.0, 2999.0};
    constexpr auto scalarMeter   = 1.0;

    // clang-format off
    STATIC_REQUIRE(distance + distance == distanceTwice);
    STATIC_REQUIRE(scalarMeter + distanceMeter == distanceMeter + scalarMeter);
    STATIC_REQUIRE(distance - distance == Vector3d{0});
    STATIC_REQUIRE(distanceMeter - scalarMeter == distanceDiff);
    // clang-format on
}

TEST_CASE("Vector3d - Binary * and / Operators")
{
    constexpr auto distance = Vector3d{1.0, 2.0, 4.0};
    constexpr auto time     = Vector3d{2.0, 1.0, 4.0};
    constexpr auto speed    = Vector3d{0.5, 2.0, 1.0};
    constexpr auto area     = Vector3d{1.0, 4.0, 16.0};

    STATIC_REQUIRE(area == distance * distance);
    STATIC_REQUIRE(2 * distance == distance * 2);
    STATIC_REQUIRE(speed == distance / time);
    STATIC_REQUIRE(distance / 2 == distance * 0.5);
}

TEST_CASE("Vector3d - Norm Functions")
{
    constexpr auto time = Vector3d{2.0, 3.0, 6.0};

    STATIC_REQUIRE(normSquared(time) == (49.0));
    REQUIRE(norm(time) == (7.0));
}

TEST_CASE("Vector3d - ostream <<")
{
    constexpr auto distance = Vector3d{1, 2, 3};

    std::ostringstream oss;
    oss << distance;

    REQUIRE(oss.str() == "1 2 3");
}

TEST_CASE("Vector3d - Nested Vector3d Indexing Operator")
{
    constexpr auto pos1 = Vector3d{1, 2, 3};
    constexpr auto pos2 = Vector3d{4, 5, 6};
    constexpr auto pos3 = Vector3d{7, 8, 9};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    STATIC_REQUIRE(pos[0] == pos1);
    STATIC_REQUIRE(pos[1] == pos2);
    STATIC_REQUIRE(pos[2] == pos3);

    STATIC_REQUIRE(pos[0][0] == 1);
    STATIC_REQUIRE(pos[0][1] == 2);
    STATIC_REQUIRE(pos[0][2] == 3);

    STATIC_REQUIRE(pos[1][0] == 4);
    STATIC_REQUIRE(pos[1][1] == 5);
    STATIC_REQUIRE(pos[1][2] == 6);

    STATIC_REQUIRE(pos[2][0] == 7);
    STATIC_REQUIRE(pos[2][1] == 8);
    STATIC_REQUIRE(pos[2][2] == 9);
}

TEST_CASE("Vector3d - Nested Vector3d Unary +/-")
{
    constexpr auto pos1        = Vector3d{1, 2, 3};
    constexpr auto pos2        = Vector3d{4, 5, 6};
    constexpr auto pos3        = Vector3d{7, 8, 9};
    constexpr auto pos         = Vector3d{pos1, pos2, pos3};
    constexpr auto negativePos = Vector3d{-pos1, -pos2, -pos3};

    STATIC_REQUIRE(+pos == pos);
    STATIC_REQUIRE(-pos == negativePos);
}

TEST_CASE("Vector3d - Nested Vector3d Binary +")
{
    constexpr auto pos1 = Vector3d{1, 2, 3};
    constexpr auto pos2 = Vector3d{4, 5, 6};
    constexpr auto pos3 = Vector3d{7, 8, 9};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    constexpr auto shift1 = Vector3d{10, 11, 12};
    constexpr auto shift2 = Vector3d{13, 14, 15};
    constexpr auto shift3 = Vector3d{16, 17, 18};
    constexpr auto shift  = Vector3d{shift1, shift2, shift3};

    constexpr auto pos1_shifted = Vector3d{11, 13, 15};
    constexpr auto pos2_shifted = Vector3d{17, 19, 21};
    constexpr auto pos3_shifted = Vector3d{23, 25, 27};
    constexpr auto pos_shifted =
        Vector3d{pos1_shifted, pos2_shifted, pos3_shifted};

    STATIC_REQUIRE(pos + shift == pos_shifted);
    STATIC_REQUIRE(shift + pos == pos_shifted);
}

TEST_CASE("Vector3d - Nested Vector3d Binary -")
{
    constexpr auto pos1 = Vector3d{10, 11, 12};
    constexpr auto pos2 = Vector3d{13, 14, 15};
    constexpr auto pos3 = Vector3d{16, 17, 18};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    constexpr auto shift1 = Vector3d{9, 8, 7};
    constexpr auto shift2 = Vector3d{6, 5, 4};
    constexpr auto shift3 = Vector3d{3, 2, 1};
    constexpr auto shift  = Vector3d{shift1, shift2, shift3};

    constexpr auto pos1_shifted = Vector3d{1, 3, 5};
    constexpr auto pos2_shifted = Vector3d{7, 9, 11};
    constexpr auto pos3_shifted = Vector3d{13, 15, 17};
    constexpr auto pos_shifted =
        Vector3d{pos1_shifted, pos2_shifted, pos3_shifted};

    STATIC_REQUIRE(pos - shift == pos_shifted);
}

TEST_CASE("Vector3d - Nested Vector3d Binary *")
{
    constexpr auto pos1 = Vector3d{1, 2, 3};
    constexpr auto pos2 = Vector3d{4, 5, 6};
    constexpr auto pos3 = Vector3d{7, 8, 9};
    constexpr auto pos  = Vector3d{pos1, pos2, pos3};

    constexpr auto factor1 = Vector3d{10, 11, 12};
    constexpr auto factor2 = Vector3d{13, 14, 15};
    constexpr auto factor3 = Vector3d{16, 17, 18};
    constexpr auto factor  = Vector3d{factor1, factor2, factor3};

    constexpr auto pos1_scaled = Vector3d{10, 22, 36};
    constexpr auto pos2_scaled = Vector3d{52, 70, 90};
    constexpr auto pos3_scaled = Vector3d{112, 136, 162};
    constexpr auto pos_scaled = Vector3d{pos1_scaled, pos2_scaled, pos3_scaled};

    STATIC_REQUIRE(pos * factor == pos_scaled);
    STATIC_REQUIRE(factor * pos == pos_scaled);
}

TEST_CASE("Vector3d - Nested Vector3d Binary /")
{
    constexpr auto num1 = Vector3d{1, 4, 9};
    constexpr auto num2 = Vector3d{16, 25, 36};
    constexpr auto num3 = Vector3d{49, 64, 81};
    constexpr auto num  = Vector3d{num1, num2, num3};

    constexpr auto den1 = Vector3d{1, 2, 3};
    constexpr auto den2 = Vector3d{4, 5, 6};
    constexpr auto den3 = Vector3d{7, 8, 9};
    constexpr auto den  = Vector3d{den1, den2, den3};

    constexpr auto quotient1 = Vector3d{1, 2, 3};
    constexpr auto quotient2 = Vector3d{4, 5, 6};
    constexpr auto quotient3 = Vector3d{7, 8, 9};
    constexpr auto quotient  = Vector3d{quotient1, quotient2, quotient3};

    STATIC_REQUIRE(num / den == quotient);
}
