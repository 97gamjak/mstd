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

#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "mstd/math/power.hpp"

TEST_CASE("cpow computes integer powers at compile-time", "[math][cpow]")
{
    using mstd::cpow;

    STATIC_REQUIRE(cpow<0>(2.0) == 1.0);
    STATIC_REQUIRE(cpow<1>(2.0) == 2.0);
    STATIC_REQUIRE(cpow<2>(2.0) == 4.0);
    STATIC_REQUIRE(cpow<3>(2.0) == 8.0);
    STATIC_REQUIRE(cpow<4>(2.0) == 16.0);
    STATIC_REQUIRE(cpow<5>(2.0) == 32.0);

    STATIC_REQUIRE(cpow<-1>(2.0) == 0.5);
    STATIC_REQUIRE(cpow<-2>(2.0) == 0.25);
    STATIC_REQUIRE(cpow<-3>(2.0) == 0.125);
    STATIC_REQUIRE(cpow<-4>(2.0) == 0.0625);
    STATIC_REQUIRE(cpow<-5>(2.0) == 0.03125);
}