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
#include <concepts>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "mstd/types.hpp"

namespace
{
    struct Base
    {
        int value = 1;

        [[nodiscard]] int getValue() const { return value; }
    };

    struct Derived : Base
    {
    };

    int readThroughReference(const Base &base) { return base.value; }

    // -- compile-time properties -------------------------------------------

    static_assert(std::is_trivially_copyable_v<mstd::Ref<int>>);
    static_assert(std::is_nothrow_copy_constructible_v<mstd::Ref<int>>);
    static_assert(std::is_nothrow_copy_assignable_v<mstd::Ref<int>>);
    static_assert(std::is_nothrow_move_assignable_v<mstd::ConstRef<int>>);
    static_assert(!std::is_default_constructible_v<mstd::Ref<int>>);

    // never bind to rvalues
    static_assert(!std::is_constructible_v<mstd::Ref<int>, int>);
    static_assert(!std::is_constructible_v<mstd::Ref<int>, int &&>);
    static_assert(!std::is_constructible_v<mstd::ConstRef<int>, int>);
    static_assert(!std::is_constructible_v<mstd::ConstRef<int>, int &&>);
    static_assert(!std::is_constructible_v<mstd::ConstRef<int>, const int &&>);

    // lvalues work, const lvalue only into ConstRef
    static_assert(std::is_constructible_v<mstd::Ref<int>, int &>);
    static_assert(std::is_constructible_v<mstd::ConstRef<int>, int &>);
    static_assert(std::is_constructible_v<mstd::ConstRef<int>, const int &>);
    static_assert(!std::is_constructible_v<mstd::Ref<int>, const int &>);

    // conversions between Refs
    static_assert(std::is_convertible_v<mstd::Ref<int>, mstd::ConstRef<int>>);
    static_assert(!std::is_convertible_v<mstd::ConstRef<int>, mstd::Ref<int>>);
    static_assert(std::is_convertible_v<mstd::Ref<Derived>, mstd::Ref<Base>>);
    static_assert(
        std::is_convertible_v<mstd::Ref<Derived>, mstd::ConstRef<Base>>
    );
    static_assert(!std::is_convertible_v<mstd::Ref<Base>, mstd::Ref<Derived>>);

    static_assert(std::same_as<mstd::ConstRef<int>, mstd::Ref<const int>>);
    static_assert(std::same_as<mstd::Ref<int>::type, int>);

    static_assert(
        std::same_as<decltype(std::declval<mstd::Ref<int>>().get()), int &>
    );
    static_assert(std::same_as<
                  decltype(std::declval<mstd::ConstRef<int>>().get()),
                  const int &>);
    static_assert(std::same_as<
                  decltype(*std::declval<mstd::ConstRef<int>>()),
                  const int &>);
}   // namespace

// -- runtime behavior ---------------------------------------------------

TEST_CASE("mstd::Ref refers to the original object", "[mstd][types][ref]")
{
    int            value = 5;
    mstd::Ref<int> ref(value);

    REQUIRE(ref.get() == 5);
    REQUIRE(&ref.get() == &value);
    REQUIRE(*ref == 5);

    *ref = 7;   // non-const Ref allows mutation
    REQUIRE(value == 7);
}

TEST_CASE(
    "mstd::ConstRef reads but tracks later changes",
    "[mstd][types][ref]"
)
{
    int                 value = 1;
    mstd::ConstRef<int> ref(value);

    value = 2;

    REQUIRE(ref.get() == 2);
    REQUIRE(&ref.get() == &value);
}

TEST_CASE(
    "mstd::Ref provides pointer-like member access",
    "[mstd][types][ref]"
)
{
    Base                 base;
    mstd::ConstRef<Base> constRef(base);
    mstd::Ref<Base>      ref(base);

    REQUIRE(constRef->value == 1);
    REQUIRE(constRef->getValue() == 1);

    ref->value = 9;
    REQUIRE(constRef->value == 9);
    REQUIRE(std::addressof(*constRef) == std::addressof(base));
}

TEST_CASE("mstd::Ref converts implicitly to T&", "[mstd][types][ref]")
{
    Base                 base;
    mstd::ConstRef<Base> ref(base);

    REQUIRE(readThroughReference(ref) == 1);

    const Base &asRef = ref;
    REQUIRE(&asRef == &base);
}

TEST_CASE("mstd::Ref is rebindable and assignable", "[mstd][types][ref]")
{
    int a = 1;
    int b = 2;

    mstd::Ref<int> ref(a);
    ref = mstd::Ref<int>(b);

    REQUIRE(&ref.get() == &b);
    REQUIRE(a == 1);   // assignment rebinds, it does not assign through
    REQUIRE(b == 2);
}

TEST_CASE("mstd::Ref copies refer to the same object", "[mstd][types][ref]")
{
    int            value = 3;
    mstd::Ref<int> original(value);
    mstd::Ref<int> copy = original;

    *copy = 4;

    REQUIRE(value == 4);
    REQUIRE(&original.get() == &copy.get());
}

TEST_CASE(
    "mstd::Ref converts to ConstRef and to base",
    "[mstd][types][ref]"
)
{
    Derived                 derived;
    mstd::Ref<Derived>      derivedRef(derived);
    mstd::Ref<Base>         baseRef      = derivedRef;
    mstd::ConstRef<Base>    constRef     = derivedRef;
    mstd::ConstRef<Derived> constDerived = derivedRef;

    REQUIRE(&baseRef.get() == static_cast<Base *>(&derived));
    REQUIRE(&constRef.get() == static_cast<const Base *>(&derived));
    REQUIRE(&constDerived.get() == &derived);
}

TEST_CASE(
    "mstd::Ref deduction guide deduces constness",
    "[mstd][types][ref]"
)
{
    int       value      = 1;
    const int constValue = 2;

    mstd::Ref ref(value);
    mstd::Ref constRef(constValue);

    static_assert(std::same_as<decltype(ref), mstd::Ref<int>>);
    static_assert(std::same_as<decltype(constRef), mstd::Ref<const int>>);

    REQUIRE(ref.get() == 1);
    REQUIRE(constRef.get() == 2);
}

TEST_CASE(
    "mstd::Ref works as an assignable class member",
    "[mstd][types][ref]"
)
{
    struct Consumer
    {
        mstd::ConstRef<std::string> name;
    };

    const std::string first  = "first";
    const std::string second = "second";

    Consumer a{first};
    Consumer b{second};

    a = b;   // would not compile with a plain const T& member

    REQUIRE(&a.name.get() == &second);
    REQUIRE(a.name->size() == 6);
}

TEST_CASE("mstd::Ref can live in containers", "[mstd][types][ref]")
{
    int a = 1;
    int b = 2;

    std::vector<mstd::Ref<int>> refs;
    refs.emplace_back(a);
    refs.emplace_back(b);

    for (const auto &ref : refs)
        *ref += 10;

    REQUIRE(a == 11);
    REQUIRE(b == 12);
}

TEST_CASE(
    "mstd::Ref is usable in constant expressions",
    "[mstd][types][ref]"
)
{
    static constexpr int          value = 42;
    constexpr mstd::ConstRef<int> ref(value);

    static_assert(ref.get() == 42);
    static_assert(*ref == 42);

    REQUIRE(ref.get() == 42);
}
