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

        REQUIRE(Velocity<m, s>(1.0).get_unit() == "ms^-1");
    }
}