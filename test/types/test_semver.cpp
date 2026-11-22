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
#include <algorithm>
#include <vector>

#include "mstd/exceptions.hpp"
#include "mstd/types/semver/semver.hpp"

using mstd::InvalidArgument;
using mstd::SemVerWithoutPrefix;
using mstd::SemVerWithPrefix;
using mstd::SemVerBase;

TEST_CASE("SemVer parses and round-trips", "[types][semver]")
{
    REQUIRE(SemVerWithPrefix::from_string("v1.2.3").to_string() == "v1.2.3");
    REQUIRE(SemVerWithoutPrefix::from_string("1.2.3").to_string() == "1.2.3");
    REQUIRE(
        SemVerWithPrefix::from_string("v1.2.3+abc123").to_string() ==
        "v1.2.3+abc123"
    );

    const auto v = SemVerWithoutPrefix::from_string("10.20.30+deadbeef");
    REQUIRE(v.base() == SemVerBase{10, 20, 30});
    REQUIRE(v.hash() == "deadbeef");
}

TEST_CASE("SemVer rejects malformed input", "[types][semver]")
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

TEST_CASE("SemVer ordering ignores the hash", "[types][semver]")
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

TEST_CASE("SemVer bases can be sorted regardless of hash", "[types][semver]")
{
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers)
    std::vector<SemVerBase> bases{{1, 2, 4}, {0, 9, 9}, {1, 2, 3}};
    std::sort(bases.begin(), bases.end());
    REQUIRE(bases.front() == SemVerBase{0, 9, 9});
    REQUIRE(bases.back() == SemVerBase{1, 2, 4});
}

TEST_CASE("SemVer is usable in constant expressions", "[types][semver]")
{
    constexpr auto a = SemVerWithPrefix::from_string("v1.2.3+abc123");
    constexpr auto b = SemVerWithPrefix(SemVerBase{1, 2, 3}, "abc123");
    constexpr auto c = SemVerWithPrefix::from_string("v1.3.0");

    static_assert(a == b);
    static_assert(a < c);
    static_assert(a.base() == SemVerBase{1, 2, 3});
    static_assert(a.hash() == "abc123");
    static_assert(!c.has_hash());
    static_assert(
        std::is_eq(a <=> b) && (a <=> SemVerWithPrefix::from_string("v1.2.3")) ==
                                   std::partial_ordering::unordered
    );

    REQUIRE(a.to_string() == "v1.2.3+abc123");
}

TEST_CASE("SemVer enforces the hash capacity", "[types][semver]")
{
    const std::string ok(SemVerWithoutPrefix::maxHashLength, 'a');
    const std::string tooLong = ok + "a";

    REQUIRE(SemVerWithoutPrefix::from_string("1.0.0+" + ok).hash() == ok);
    REQUIRE_THROWS_AS(
        SemVerWithoutPrefix::from_string("1.0.0+" + tooLong), InvalidArgument
    );
}
