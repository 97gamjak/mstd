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

#include "mstd/ratio.hpp"
#include "test_utils.hpp"

using namespace mstd;
using namespace mstd::details;

TEST_CASE("PowRatio")
{
    using pow_ratio = PowRatio<>;
    MSTD_STATIC_REQUIRE(pow_ratio::num == 1);
    MSTD_STATIC_REQUIRE(pow_ratio::den == 1);
    MSTD_STATIC_REQUIRE(pow_ratio::p == 1);
    MSTD_STATIC_REQUIRE(pow_ratio::q == 1);

    using pow_ratio2 = PowRatio<2>;
    MSTD_STATIC_REQUIRE(pow_ratio2::num == 2);
    MSTD_STATIC_REQUIRE(pow_ratio2::den == 1);
    MSTD_STATIC_REQUIRE(pow_ratio2::p == 1);
    MSTD_STATIC_REQUIRE(pow_ratio2::q == 1);

    using pow_ratio3 = PowRatio<2, 3, 2, 4>;
    MSTD_STATIC_REQUIRE(pow_ratio3::num == 2);
    MSTD_STATIC_REQUIRE(pow_ratio3::den == 3);
    MSTD_STATIC_REQUIRE(pow_ratio3::_gcd == 2);
    MSTD_STATIC_REQUIRE(pow_ratio3::p == 1);
    MSTD_STATIC_REQUIRE(pow_ratio3::q == 2);
}