#include <array>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>

#include "mstd/math/power.hpp"

TEST_CASE("cpow computes integer powers at compile-time", "[math][cpow]")
{
    using mstd::cpow;

    STATIC_REQUIRE(cpow<0>(2.0) == 1.0);
    STATIC_REQUIRE(cpow<1>(2.0) == 2.0);
    STATIC_REQUIRE(cpow<2>(2.0) == 4.0);
    STATIC_REQUIRE(cpow<3>(2.0) == 8.0);
    STATIC_REQUIRE(cpow<4>(2.0) == 16.0);
    STATIC_REQUIRE(cpow<5>(2.0) == 32.0);

    STATIC_REQUIRE(cpow<-1>(2.0) == 0.5);
    STATIC_REQUIRE(cpow<-2>(2.0) == 0.25);
    STATIC_REQUIRE(cpow<-3>(2.0) == 0.125);
    STATIC_REQUIRE(cpow<-4>(2.0) == 0.0625);
    STATIC_REQUIRE(cpow<-5>(2.0) == 0.03125);
}