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

#include "mstd/units.hpp"

TEST_CASE("mstd::units::scale_tags", "[units][scale_tags]")
{
    using namespace mstd;

    SECTION("random")
    {
        REQUIRE(is_L_v<L<std::ratio<1>>>);
        REQUIRE(is_T_v<T<std::ratio<1>>>);
        REQUIRE(Velocity<m, s>(1.0).value() == 1.0);
        REQUIRE(Velocity<km, h>(1.0).value() == 1.0);
        REQUIRE(Velocity<km, h>(1.0).si() == 1 / 3.6);

        REQUIRE(Velocity<m, s>(1.0).get_unit_string() == "ms^-1");
        REQUIRE(Velocity<km, h>(100.0).get_unit_string() == "kmh^-1");
        REQUIRE(
            Quantity<double, D_Mass, MassPack<mg>>::get_unit_string() == "mg"
        );
        REQUIRE(
            Quantity<double, D_Time, TimePack<ms>>::get_unit_string() == "ms"
        );
        REQUIRE(
            Quantity<double, D_Time, TimePack<min>>(1333).get_unit_string() ==
            "min"
        );

        REQUIRE(
            Quantity<double, D_Time, TimePack<yr>>(1333).to_string() ==
            "1333.000000"
        );
        REQUIRE(
            Quantity<double, D_Time, TimePack<yr>>(1333.0).to_unit_string(3) ==
            "1333.000 yr"
        );
    }
}