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

#include "mstd/units/new_units.hpp"

namespace
{
    // -- per_cm and per_bar are the one / cm and one / bar units -------------

    static_assert(mstd::units::per_cm == mstd::units::one / mstd::units::cm);
    static_assert(mstd::units::per_bar == mstd::units::one / mstd::units::bar);
}   // namespace

TEST_CASE(
    "bar converts to pascal using the 1 bar = 100000 Pa definition",
    "[units][new_units]"
)
{
    const auto pressure = 1.0 * mstd::units::bar;

    REQUIRE(
        pressure.numerical_value_in(mstd::units::Pa) == Catch::Approx(100'000.0)
    );

    const auto pressure2 = 2.5 * mstd::units::bar;

    REQUIRE(
        pressure2.numerical_value_in(mstd::units::Pa) ==
        Catch::Approx(250'000.0)
    );
}

TEST_CASE("bar round-trips through pascal", "[units][new_units]")
{
    const auto original = 3.14 * mstd::units::bar;
    const auto inPa     = original.in(mstd::units::Pa);
    const auto back     = inPa.in(mstd::units::bar);

    REQUIRE(
        back.numerical_value_in(mstd::units::bar) ==
        Catch::Approx(original.numerical_value_in(mstd::units::bar))
    );
}

TEST_CASE("mstd::units::per_cm behaves like one / cm", "[units][new_units]")
{
    const auto viaPerCm = 5.0 * mstd::units::per_cm;
    const auto viaOneCm = 5.0 * (mstd::units::one / mstd::units::cm);

    REQUIRE(
        viaPerCm.numerical_value_in(mstd::units::per_cm) ==
        viaOneCm.numerical_value_in(mstd::units::one / mstd::units::cm)
    );
}

TEST_CASE("mstd::units::per_bar behaves like one / bar", "[units][new_units]")
{
    const auto viaPerBar = 5.0 * mstd::units::per_bar;
    const auto viaOneBar = 5.0 * (mstd::units::one / mstd::units::bar);

    REQUIRE(
        viaPerBar.numerical_value_in(mstd::units::per_bar) ==
        viaOneBar.numerical_value_in(mstd::units::one / mstd::units::bar)
    );
}

TEST_CASE("mstd::units::per_bar is the reciprocal of bar", "[units][new_units]")
{
    const auto pressure   = 4.0 * mstd::units::bar;
    const auto reciprocal = 1.0 * mstd::units::per_bar;

    REQUIRE(
        (pressure * reciprocal).numerical_value_in(mstd::units::one) ==
        Catch::Approx(4.0)
    );
}

TEST_CASE("mstd::units::per_Pa behaves like one / Pa", "[units][new_units]")
{
    const auto viaPerPa = 5.0 * mstd::units::per_Pa;
    const auto viaOnePa = 5.0 * (mstd::units::one / mstd::units::Pa);

    REQUIRE(
        viaPerPa.numerical_value_in(mstd::units::per_Pa) ==
        viaOnePa.numerical_value_in(mstd::units::one / mstd::units::Pa)
    );
}

TEST_CASE("kcal converts to joule using 1 kcal = 4184 J", "[units][new_units]")
{
    const auto energy = 1.0 * mstd::units::kcal;

    REQUIRE(
        energy.numerical_value_in(mstd::units::J) == Catch::Approx(4184.0)
    );
}

TEST_CASE(
    "kcal_per_mol converts to J/mol using 1 kcal/mol = 4184 J/mol",
    "[units][new_units]"
)
{
    const auto energy = 2.0 * mstd::units::kcal_per_mol;

    REQUIRE(
        energy.numerical_value_in(mstd::units::J / mstd::units::mol) ==
        Catch::Approx(8368.0)
    );
    REQUIRE(
        energy.numerical_value_in(mstd::units::kJ / mstd::units::mol) ==
        Catch::Approx(8.368)
    );
}

TEST_CASE("angstrom converts to metre using 1 A = 1e-10 m", "[units][new_units]")
{
    const auto length = 3.0 * mstd::units::angstrom;

    REQUIRE(
        length.numerical_value_in(mstd::units::m) == Catch::Approx(3.0e-10)
    );
    REQUIRE(
        length.numerical_value_in(mstd::units::nm) == Catch::Approx(0.3)
    );
}
