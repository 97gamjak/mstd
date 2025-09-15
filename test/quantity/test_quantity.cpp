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

#include "mstd/quantity.hpp"

TEST_CASE("quantity", "[units]")
{
    using namespace mstd::units;
    using namespace mstd::units::literals;
    const auto length = Length<m>{1};
    REQUIRE(length.value() == 1);
    REQUIRE(length.baseValue() == 1);
    const auto length_cm = Length<cm>{100};
    REQUIRE(length_cm.baseValue() == 1);
    REQUIRE(length_cm.value() == 100);
    REQUIRE(same_dimension_v<cm, m>);
    REQUIRE(length_cm == length);
    REQUIRE(length_cm + length == Length<m>{2.00});
    const auto new_length = length + length_cm;
    REQUIRE(new_length.value() == 200);
    REQUIRE(new_length.baseValue() == 2.00);
    REQUIRE(to<m>(new_length).value() == 2.0);
    const auto mass = Mass<kg>{2};
    REQUIRE(mass.value() == 2);
    // const auto c_velocity = Velocity<c>{1};
}