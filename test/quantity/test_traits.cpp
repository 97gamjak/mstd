#include <catch2/catch_test_macros.hpp>

#include "mstd/quantity.hpp"
#include "mstd/type_traits.hpp"
#include "test_utils.hpp"

TEST_CASE("type_trait", "[units]")
{
    using namespace mstd;
    MSTD_STATIC_REQUIRE(is_unit_v<literals::cm>);
    MSTD_STATIC_REQUIRE(is_unit_v<literals::m>);
    MSTD_STATIC_REQUIRE(is_unit_v<literals::kg>);
}