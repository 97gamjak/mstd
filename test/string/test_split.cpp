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
#include <string>
#include <string_view>
#include <vector>

#include "mstd/string.hpp"

TEST_CASE("split splits at a delimiter", "[string][split]")
{
    using V = std::vector<std::string>;

    CHECK(mstd::split("a,b,c", ",") == V{"a", "b", "c"});
    CHECK(mstd::split("a::b::c", "::") == V{"a", "b", "c"});
    CHECK(mstd::split("abc", ",") == V{"abc"});
}

TEST_CASE("split keeps empty parts", "[string][split]")
{
    using V = std::vector<std::string>;

    CHECK(mstd::split("a,,b", ",") == V{"a", "", "b"});
    CHECK(mstd::split(",a,", ",") == V{"", "a", ""});
    CHECK(mstd::split("", ",") == V{""});
}

TEST_CASE("split with empty delimiter returns whole string", "[string][split]")
{
    CHECK(mstd::split("abc", "") == std::vector<std::string>{"abc"});
}

TEST_CASE("split with transform", "[string][split]")
{
    auto res = mstd::split<int>(
        "1,2,3",
        ",",
        [](std::string_view s) { return std::stoi(std::string(s)); }
    );

    CHECK(res == std::vector<int>{1, 2, 3});
}

TEST_CASE("split is the inverse of join", "[string][split]")
{
    const std::string s = "x;;y;z";
    CHECK(mstd::join(mstd::split(s, ";"), ";") == s);
}
