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