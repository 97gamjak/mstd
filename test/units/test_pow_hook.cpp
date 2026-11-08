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
#include <type_traits>

#include "mstd/units.hpp"

TEST_CASE(
    "mstd::pow/sqrt/cbrt dispatch to std:: for arithmetic types",
    "[units][pow_hook]"
)
{
    REQUIRE(mstd::pow<3>(2.0) == Catch::Approx(8.0));
    REQUIRE(mstd::pow<-2>(2.0) == Catch::Approx(0.25));
    REQUIRE(mstd::pow<1, 2>(9.0) == Catch::Approx(3.0));
    REQUIRE(mstd::pow(2.0, 0.5) == Catch::Approx(std::sqrt(2.0)));
    REQUIRE(mstd::pow(2, 3) == Catch::Approx(8.0));
    REQUIRE(mstd::sqrt(9.0) == Catch::Approx(std::sqrt(9.0)));
    REQUIRE(mstd::cbrt(27.0) == Catch::Approx(std::cbrt(27.0)));
}

TEST_CASE(
    "mstd::pow/sqrt/cbrt keep quantities and change their dimension",
    "[units][pow_hook]"
)
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;

    const auto length = 3.0 * m;
    const auto area   = mstd::pow<2>(length);
    const auto volume = mstd::pow<3>(length);

    STATIC_REQUIRE(std::is_same_v<decltype(area), const quantity<pow<2>(m), double>>);
    REQUIRE(area.numerical_value_in(m * m) == Catch::Approx(9.0));
    REQUIRE(volume.numerical_value_in(m * m * m) == Catch::Approx(27.0));

    REQUIRE(mstd::sqrt(area).numerical_value_in(m) == Catch::Approx(3.0));
    REQUIRE(mstd::cbrt(volume).numerical_value_in(m) == Catch::Approx(3.0));
    REQUIRE(mstd::pow<1, 2>(area).numerical_value_in(m) == Catch::Approx(3.0));
}
