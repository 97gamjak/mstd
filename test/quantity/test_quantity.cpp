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

#include <type_traits>

#include "mstd/quantity.hpp"

TEST_CASE("quantity construction and accessors", "[units]")
{
    using namespace mstd::units;
    using namespace mstd::units::literals;

    constexpr Length<m> one_meter{1.0};
    STATIC_REQUIRE(one_meter.value() == 1.0);
    STATIC_REQUIRE(one_meter.baseValue() == 1.0);

    constexpr Length<cm> hundred_centimetres{100.0};
    STATIC_REQUIRE(hundred_centimetres.value() == 100.0);
    STATIC_REQUIRE(hundred_centimetres.baseValue() == 1.0);

    const Length<cm> from_base{Length<cm>::from_base_tag, 2.0};
    REQUIRE(from_base.baseValue() == Catch::Approx(2.0));
    REQUIRE(from_base.value() == Catch::Approx(200.0));

    const auto via_factory = qty<cm>(5.0);
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(via_factory)>, Length<cm>>);
    REQUIRE(via_factory.value() == Catch::Approx(5.0));
    REQUIRE(via_factory.baseValue() == Catch::Approx(0.05));

    const Energy<cal> one_calorie{1.0};
    REQUIRE(one_calorie.value() == Catch::Approx(1.0));
    REQUIRE(one_calorie.baseValue() == Catch::Approx(4.184));
}

TEST_CASE("quantity conversions and comparisons", "[units]")
{
    using namespace mstd::units;
    using namespace mstd::units::literals;

    STATIC_REQUIRE(same_dimension_v<cm, m>);
    STATIC_REQUIRE_FALSE(same_dimension_v<cm, kg>);
    STATIC_REQUIRE(compatible_units_v<cm, mm>);
    STATIC_REQUIRE_FALSE(compatible_units_v<cm, s>);

    const Length<cm> length_cm{250.0};
    const auto length_m = to<m>(length_cm);
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(length_m)>, Length<m>>);
    REQUIRE(length_m.value() == Catch::Approx(2.5));
    REQUIRE(length_m.baseValue() == Catch::Approx(2.5));

    const auto length_mm = to<mm>(length_m);
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(length_mm)>, Length<mm>>);
    REQUIRE(length_mm.value() == Catch::Approx(2500.0));

    const Velocity<kmh> highway_speed{72.0};
    const auto highway_speed_mps = to<mps>(highway_speed);
    STATIC_REQUIRE(
        std::is_same_v<std::decay_t<decltype(highway_speed_mps)>, Velocity<mps>>);
    REQUIRE(highway_speed_mps.value() == Catch::Approx(20.0));

    REQUIRE(Length<m>{1.0} == Length<cm>{100.0});
    REQUIRE_FALSE(Length<m>{1.0} == Length<cm>{99.0});
    REQUIRE_FALSE(Length<m>{1.0} == Mass<kg>{1.0});

    const Length<m, float> numerator{9.0F};
    const Length<cm, float> denominator{100.0F};
    REQUIRE(quantity_cast<float>(numerator, denominator) == Catch::Approx(9.0F));

    const Length<m, float> zero_denom{0.0F};
    REQUIRE(quantity_cast<float>(numerator, zero_denom) == Catch::Approx(0.0F));
}

TEST_CASE("quantity arithmetic for compatible units", "[units]")
{
    using namespace mstd::units;
    using namespace mstd::units::literals;

    const Length<m> a{2.0};
    const Length<m, int> b{3};
    const auto sum_same = a + b;
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(sum_same)>, Length<m>>);
    REQUIRE(sum_same.value() == Catch::Approx(5.0));
    REQUIRE(sum_same.baseValue() == Catch::Approx(5.0));

    const Length<cm> cm_value{50.0};
    const Length<m> m_value{2.0};
    const auto sum_mixed = cm_value + m_value;
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(sum_mixed)>, Length<cm>>);
    REQUIRE(sum_mixed.value() == Catch::Approx(250.0));
    REQUIRE(sum_mixed.baseValue() == Catch::Approx(2.5));

    const auto diff_mixed = cm_value - m_value;
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(diff_mixed)>, Length<cm>>);
    REQUIRE(diff_mixed.value() == Catch::Approx(-150.0));
    REQUIRE(diff_mixed.baseValue() == Catch::Approx(-1.5));

    const auto difference_same = m_value - Length<m>{0.5};
    REQUIRE(difference_same.value() == Catch::Approx(1.5));
    REQUIRE(difference_same.baseValue() == Catch::Approx(1.5));

    const auto velocity = Length<m>{10.0} / Time<s>{2.0};
    STATIC_REQUIRE(std::is_same_v<typename decltype(velocity)::unit, mps>);
    REQUIRE(velocity.value() == Catch::Approx(5.0));

    const auto area = Length<m>{3.0} * Length<m>{2.0};
    STATIC_REQUIRE(
        std::is_same_v<
            typename decltype(area)::unit,
            unit_mul<m, m>>);
    REQUIRE(area.value() == Catch::Approx(6.0));

    const auto energy = Force<N>{2.0} * Length<m>{3.0};
    STATIC_REQUIRE(std::is_same_v<typename decltype(energy)::unit, unit_mul<N, m>>);
    REQUIRE(energy.value() == Catch::Approx(6.0));
}

TEST_CASE("quantity interaction with scalars", "[units]")
{
    using namespace mstd::units;
    using namespace mstd::units::literals;

    const Length<m> base_length{1.5};
    const auto stretched = base_length * 2;
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(stretched)>, Length<m>>);
    REQUIRE(stretched.value() == Catch::Approx(3.0));

    const auto stretched_commuted = 2.0 * base_length;
    STATIC_REQUIRE(
        std::is_same_v<std::decay_t<decltype(stretched_commuted)>, Length<m>>);
    REQUIRE(stretched_commuted.value() == Catch::Approx(3.0));

    const auto halved = stretched / 2;
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(halved)>, Length<m>>);
    REQUIRE(halved.value() == Catch::Approx(1.5));

    const auto mass_scaled = 3 * Mass<kg>{4.0};
    STATIC_REQUIRE(std::is_same_v<std::decay_t<decltype(mass_scaled)>, Mass<kg>>);
    REQUIRE(mass_scaled.value() == Catch::Approx(12.0));

    const auto acceleration = Velocity<mps>{10.0} / Time<s>{2.0};
    STATIC_REQUIRE(
        std::is_same_v<
            typename decltype(acceleration)::unit,
            unit_div<mps, s>>);
    REQUIRE(acceleration.value() == Catch::Approx(5.0));

    const auto dimensionless_ratio = Velocity<mps>{50.0} / Velocity<mps>{10.0};
    REQUIRE(dimensionless_ratio.baseValue() == Catch::Approx(5.0));
    STATIC_REQUIRE(
        same_dimension_v<
            typename decltype(dimensionless_ratio)::unit,
            unitless>);
}
