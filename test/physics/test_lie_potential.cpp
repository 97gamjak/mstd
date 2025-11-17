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

#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "mstd/physics/potentials/lie_potential.hpp"

TEST_CASE("LiePotential evaluates energy/force for arbitrary exponents", "[lie_potential]")
{
    using mstd::LiePotential;

    constexpr double coeff1 = 2.0;
    constexpr double coeff2 = 0.75;
    const LiePotential<4, 8, double> potential(coeff1, coeff2);

    const std::array<double, 3> radii{0.75, 1.5, 3.25};

    for (double r : radii)
    {
        const double expectedEnergy = -coeff1 / std::pow(r, 4) + coeff2 / std::pow(r, 8);
        const double expectedForce  = 4 * coeff1 / r / std::pow(r, 4) -
                                     8 * coeff2 / r / std::pow(r, 8);

        REQUIRE(potential.evalEnergy(r) == Catch::Approx(expectedEnergy));
        REQUIRE(potential.evalForce(r) == Catch::Approx(expectedForce));

        const auto [energy, force] = potential.eval(r);
        REQUIRE(energy == Catch::Approx(expectedEnergy));
        REQUIRE(force == Catch::Approx(expectedForce));
    }
}

TEST_CASE("LJPotential optimized helpers match analytic expression", "[lie_potential]")
{
    using mstd::LJPotential;

    constexpr double coeff1 = 1.25;
    constexpr double coeff2 = 0.5;
    const LJPotential<double> potential(coeff1, coeff2);

    const std::array<double, 2> radii{0.9, 2.2};

    for (double r : radii)
    {
        const double r6        = std::pow(r, 6);
        const double r12       = std::pow(r, 12);
        const double coeff1r6  = coeff1 / r6;
        const double coeff2r12 = coeff2 / r12;
        const double expectedEnergy = -coeff1r6 + coeff2r12;
        const double expectedForce  = 6 * coeff1r6 / r - 12 * coeff2r12 / r;

        REQUIRE(potential.evalEnergy(r) == Catch::Approx(expectedEnergy));
        REQUIRE(potential.evalForce(r) == Catch::Approx(expectedForce));

        const auto [energy, force] = potential.eval(r);
        REQUIRE(energy == Catch::Approx(expectedEnergy));
        REQUIRE(force == Catch::Approx(expectedForce));
    }
}
