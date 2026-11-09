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
#include <limits>
#include <type_traits>

#include "mstd/units.hpp"

TEST_CASE("mstd::max/min work for arithmetic types", "[units][minmax]")
{
    REQUIRE(mstd::max(1, 2) == 2);
    REQUIRE(mstd::min(1, 2) == 1);
    REQUIRE(mstd::max(1, 2.5) == Catch::Approx(2.5));
    REQUIRE(mstd::min(1, 2.5) == Catch::Approx(1.0));
}

TEST_CASE("mstd::max/min work for quantities", "[units][minmax]")
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;

    const auto a = 1.0 * m;
    const auto b = 150.0 * cm;
    REQUIRE(mstd::max(a, b).numerical_value_in(cm) == Catch::Approx(150.0));
    REQUIRE(mstd::min(a, b).numerical_value_in(cm) == Catch::Approx(100.0));
}

TEST_CASE(
    "mstd::max/min mix doubles and dimensionless quantities",
    "[units][minmax]"
)
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;

    const auto ratio = (2.0 * m) / (8.0 * m);
    const double hi  = mstd::max(1.0, ratio);
    const double lo  = mstd::min(1.0, ratio);
    REQUIRE(hi == Catch::Approx(1.0));
    REQUIRE(lo == Catch::Approx(0.25));
}

TEST_CASE("mstd::exp/log", "[units][exp_log]")
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;

    REQUIRE(mstd::exp(1.0) == Catch::Approx(std::exp(1.0)));
    REQUIRE(mstd::log(2.0) == Catch::Approx(std::log(2.0)));

    const auto ratio = (2.0 * m) / (8.0 * m);
    const double e   = mstd::exp(ratio);
    const double l   = mstd::log(ratio);
    REQUIRE(e == Catch::Approx(std::exp(0.25)));
    REQUIRE(l == Catch::Approx(std::log(0.25)));
}

TEST_CASE("mstd::abs/fabs", "[units][abs]")
{
    using namespace mp_units;
    using namespace mp_units::si::unit_symbols;

    REQUIRE(mstd::abs(-3) == 3);
    REQUIRE(mstd::abs(-2.5) == Catch::Approx(2.5));
    REQUIRE(mstd::fabs(-2.5) == Catch::Approx(2.5));
    REQUIRE(mstd::fabs(-2) == Catch::Approx(2.0));

    const auto len = -150.0 * cm;
    REQUIRE(mstd::abs(len).numerical_value_in(cm) == Catch::Approx(150.0));
    REQUIRE(mstd::fabs(len).numerical_value_in(m) == Catch::Approx(1.5));

    const auto ratio = (2.0 * m) / (-8.0 * m);
    const double a   = mstd::abs(ratio).numerical_value_in(one);
    const double f   = mstd::fabs(ratio).numerical_value_in(one);
    REQUIRE(a == Catch::Approx(0.25));
    REQUIRE(f == Catch::Approx(0.25));

    // angles stay quantities, so they compare against numeric_limits<Q>
    using Angle      = decltype(1.0 * si::radian);
    const Angle diff = -1.0 * si::radian;
    static_assert(std::is_same_v<decltype(mstd::fabs(diff)), Angle>);
    REQUIRE(!(mstd::fabs(diff) < std::numeric_limits<Angle>::epsilon()));
}
