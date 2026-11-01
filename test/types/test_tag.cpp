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
#include <type_traits>

#include "mstd/types/tag.hpp"

namespace
{
    // -- compile-time properties -------------------------------------------

    static_assert(std::is_trivially_copyable_v<mstd::Tag<4>>);

    constexpr mstd::Tag abc("abc");
    static_assert(std::is_same_v<decltype(abc), const mstd::Tag<4>>);
    static_assert(abc.view() == "abc");

    constexpr mstd::Tag empty("");
    static_assert(empty.view() == "");
    static_assert(empty.view().empty());

    // same literal -> same type, usable as a NTTP
    template <mstd::Tag Name>
    struct Named
    {
        static constexpr std::string_view name = Name.view();
    };

    static_assert(Named<"same">::name == Named<"same">::name);
    static_assert(std::is_same_v<Named<"same">, Named<"same">>);
    static_assert(!std::is_same_v<Named<"same">, Named<"other">>);
}   // namespace

// -- runtime behavior ---------------------------------------------------

TEST_CASE("mstd::Tag exposes its value as a string_view", "[mstd][types][tag]")
{
    mstd::Tag<6> tag("hello");

    REQUIRE(tag.view() == "hello");
    REQUIRE(tag.view().size() == 5);
}

TEST_CASE("mstd::Tag converts to std::string", "[mstd][types][tag]")
{
    mstd::Tag<4> tag("foo");

    const std::string str = tag.to_string();

    REQUIRE(str == "foo");
    REQUIRE(str.size() == 3);
}

TEST_CASE("mstd::Tag deduces its size from the literal", "[mstd][types][tag]")
{
    mstd::Tag tag("deduced");

    static_assert(std::is_same_v<decltype(tag), mstd::Tag<8>>);
    REQUIRE(tag.view() == "deduced");
}

TEST_CASE("mstd::Tag handles an empty literal", "[mstd][types][tag]")
{
    mstd::Tag<1> tag("");

    REQUIRE(tag.view().empty());
    REQUIRE(tag.to_string().empty());
}

TEST_CASE(
    "mstd::Tag instances with the same value compare equal view/strings",
    "[mstd][types][tag]"
)
{
    mstd::Tag<4> a("bar");
    mstd::Tag<4> b("bar");

    REQUIRE(a.view() == b.view());
    REQUIRE(a.to_string() == b.to_string());
}

TEST_CASE("mstd::Tag copies are independent values", "[mstd][types][tag]")
{
    mstd::Tag<4> original("baz");
    mstd::Tag<4> copy = original;

    REQUIRE(copy.view() == "baz");
    REQUIRE(&copy != &original);
}
