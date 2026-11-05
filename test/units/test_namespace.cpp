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
#include <type_traits>

#include "mstd/units/namespace.hpp"
#include "mstd/units/new_units.hpp"

namespace
{
    // -- mstd::units exposes mp-units/isq/si unit symbols -------------------

    constexpr mstd::units::quantity<mstd::units::isq::length[mstd::units::m]>
        length = 2 * mstd::units::m;
    static_assert(length.numerical_value_in(mstd::units::m) == 2);

    // -- mstd::quantity is an alias for mp_units::quantity -------------------

    static_assert(std::is_same_v<
                   mstd::quantity<mstd::units::isq::length[mstd::units::m]>,
                   mstd::units::quantity<mstd::units::isq::length[mstd::units::m]>>);

    // -- per_cm is the one / cm unit -----------------------------------------

    static_assert(mstd::units::per_cm == mstd::units::one / mstd::units::cm);
}   // namespace

TEST_CASE("mstd::units::per_cm behaves like one / cm", "[units][namespace]")
{
    const auto viaPerCm  = 5.0 * mstd::units::per_cm;
    const auto viaOneCm  = 5.0 * (mstd::units::one / mstd::units::cm);

    REQUIRE(
        viaPerCm.numerical_value_in(mstd::units::per_cm) ==
        viaOneCm.numerical_value_in(mstd::units::one / mstd::units::cm)
    );
}

TEST_CASE("mstd::quantity constructs from unit symbols brought in by mstd::units", "[units][namespace]")
{
    const mstd::quantity<mstd::units::isq::time[mstd::units::s]> duration =
        3 * mstd::units::s;

    REQUIRE(duration.numerical_value_in(mstd::units::s) == 3);
}
