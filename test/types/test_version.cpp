#include <catch2/catch_test_macros.hpp>
#include <algorithm>
#include <vector>

#include "mstd/exceptions.hpp"
#include "mstd/types/version/version.hpp"

using mstd::InvalidArgument;
using mstd::SemVerWithoutPrefix;
using mstd::SemVerWithPrefix;
using mstd::VersionBase;

TEST_CASE("SemVer parses and round-trips", "[types][version]")
{
    REQUIRE(SemVerWithPrefix::from_string("v1.2.3").to_string() == "v1.2.3");
    REQUIRE(SemVerWithoutPrefix::from_string("1.2.3").to_string() == "1.2.3");
    REQUIRE(
        SemVerWithPrefix::from_string("v1.2.3+abc123").to_string() ==
        "v1.2.3+abc123"
    );

    const auto v = SemVerWithoutPrefix::from_string("10.20.30+deadbeef");
    REQUIRE(v.base() == VersionBase{10, 20, 30});
    REQUIRE(v.hash_extension() == "deadbeef");
}

TEST_CASE("SemVer rejects malformed input", "[types][version]")
{
    for (const char* s :
         {"1.2.3", "V1.2.3", "v1.2", "v1.2.3.4", "v1.2.3abc", "v-1.2.3",
          "v1.-2.3", "v1..3", "v1.2.3+", "v1.2.3+a b", "", "v",
          "v99999999999999999999999.0.0"})
    {
        CAPTURE(s);
        REQUIRE_THROWS_AS(SemVerWithPrefix::from_string(s), InvalidArgument);
    }
    REQUIRE_THROWS_AS(SemVerWithoutPrefix::from_string("v1.2.3"), InvalidArgument);
}

TEST_CASE("SemVer ordering ignores the hash", "[types][version]")
{
    using V = SemVerWithoutPrefix;
    const auto a = V::from_string("1.2.3");
    const auto b = V::from_string("1.2.4");
    const auto h1 = V::from_string("1.2.3+aaa");
    const auto h2 = V::from_string("1.2.3+bbb");

    REQUIRE(a < b);
    REQUIRE(h1 < b);
    REQUIRE(b > h2);

    // same base, different/missing hash: unordered, not equal
    for (const auto& [x, y] : {std::pair{h1, h2}, std::pair{a, h1}})
    {
        REQUIRE_FALSE(x == y);
        REQUIRE_FALSE(x < y);
        REQUIRE_FALSE(x > y);
        REQUIRE_FALSE(x <= y);
        REQUIRE_FALSE(x >= y);
    }

    REQUIRE(h1 == V::from_string("1.2.3+aaa"));
    REQUIRE(h1 <= V::from_string("1.2.3+aaa"));
}

TEST_CASE("SemVer bases can be sorted regardless of hash", "[types][version]")
{
    std::vector<VersionBase> bases{{1, 2, 4}, {0, 9, 9}, {1, 2, 3}};
    std::sort(bases.begin(), bases.end());
    REQUIRE(bases.front() == VersionBase{0, 9, 9});
    REQUIRE(bases.back() == VersionBase{1, 2, 4});
}
