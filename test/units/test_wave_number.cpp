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

#include "mstd/units/namespace.hpp"
#include "mstd/units/wave_number.hpp"

namespace
{
    // speed of light in cm/s, matching the precision toFreq/toWavenumber
    // compute with internally
    constexpr double speedOfLightCmPerS = 2.99792458e10;
}   // namespace

TEST_CASE(
    "toFreq converts a wave number to its equivalent frequency",
    "[units][wave_number]"
)
{
    const auto waveNumber = 1.0 * (mstd::units::one / mstd::units::cm);
    const auto frequency  = mstd::toFreq(waveNumber);

    REQUIRE(
        frequency.numerical_value_in(mstd::units::Hz) ==
        Catch::Approx(speedOfLightCmPerS).epsilon(1e-6)
    );

    const auto waveNumber2 = 10.0 * (mstd::units::one / mstd::units::cm);
    const auto frequency2  = mstd::toFreq(waveNumber2);

    REQUIRE(
        frequency2.numerical_value_in(mstd::units::Hz) ==
        Catch::Approx(10.0 * speedOfLightCmPerS).epsilon(1e-6)
    );
}

TEST_CASE(
    "toWavenumber converts a frequency to its equivalent wave number",
    "[units][wave_number]"
)
{
    const auto frequency = speedOfLightCmPerS * mstd::units::Hz;
    const auto waveNumber = mstd::toWavenumber(frequency);

    REQUIRE(
        waveNumber.numerical_value_in(mstd::units::one / mstd::units::cm) ==
        Catch::Approx(1.0).epsilon(1e-6)
    );
}

TEST_CASE(
    "toFreq and toWavenumber round-trip",
    "[units][wave_number]"
)
{
    const auto original = 123.456 * (mstd::units::one / mstd::units::cm);

    const auto roundTripped = mstd::toWavenumber(mstd::toFreq(original));

    REQUIRE(
        roundTripped.numerical_value_in(mstd::units::one / mstd::units::cm) ==
        Catch::Approx(
            original.numerical_value_in(mstd::units::one / mstd::units::cm)
        ).epsilon(1e-6)
    );
}
