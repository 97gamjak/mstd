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

#include <catch2/catch_test_macros.hpp>

#include "mstd/units/relaxation_time.hpp"
#include "mstd/units/namespace.hpp"

namespace
{
    constexpr auto tau = 2.5 * mstd::units::relaxation_time[mstd::units::s];

    // relaxation_time is isq::duration-compatible
    static_assert(
        mstd::units::QuantityOf<decltype(tau), mstd::units::isq::duration>
    );

    static_assert(tau.numerical_value_in(mstd::units::s) == 2.5);
}   // namespace

TEST_CASE(
    "mstd::units::relaxation_time quantities behave like durations",
    "[units][relaxation_time]"
)
{
    REQUIRE(tau.numerical_value_in(mstd::units::s) == 2.5);

    const auto asDuration =
        mstd::units::quantity_cast<mstd::units::isq::duration>(tau);

    REQUIRE(asDuration.numerical_value_in(mstd::units::s) == 2.5);
}
