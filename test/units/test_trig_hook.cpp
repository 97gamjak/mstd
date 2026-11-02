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

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <numbers>

#include "mstd/units.hpp"

TEST_CASE(
    "mstd::sin/cos/tan still dispatch to std:: for arithmetic types",
    "[units][trig_hook]"
)
{
    REQUIRE(mstd::sin(0.0) == Catch::Approx(std::sin(0.0)));
    REQUIRE(mstd::cos(0.0) == Catch::Approx(std::cos(0.0)));
    REQUIRE(mstd::tan(std::numbers::pi / 4) == Catch::Approx(std::tan(std::numbers::pi / 4)));
}

TEST_CASE(
    "mstd::sin/cos/tan dispatch to mp_units::si for angular quantities",
    "[units][trig_hook]"
)
{
    const auto rightAngle = 90 * mstd::units::deg;
    const auto straightAngle = 180 * mstd::units::deg;

    REQUIRE(mstd::sin(rightAngle) == Catch::Approx(1.0).epsilon(1e-9));
    REQUIRE(mstd::cos(rightAngle) == Catch::Approx(0.0).margin(1e-9));
    REQUIRE(mstd::sin(straightAngle) == Catch::Approx(0.0).margin(1e-9));
    REQUIRE(mstd::cos(straightAngle) == Catch::Approx(-1.0).epsilon(1e-9));

    const auto fortyFive = 45 * mstd::units::deg;
    REQUIRE(mstd::tan(fortyFive) == Catch::Approx(1.0).epsilon(1e-9));
}
