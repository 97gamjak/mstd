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
#include <cstdint>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "mstd/enum.hpp"
#include "mstd/type_traits.hpp"

namespace
{
    // Sequential, implicitly-valued enum (mirrors the common X-macro usage
    // without any explicit enumerator values).
#define MSTD_TEST_COLOR_LIST(X) \
    X(Red)                      \
    X(Green)                    \
    X(Blue)

    MSTD_ENUM(Color, int, MSTD_TEST_COLOR_LIST)

    // Mixed enum: some enumerators have explicit values (including gaps and
    // a non-zero start), others fall back to "previous value + 1", exactly
    // like mstd/quantity/enums.hpp does in production code.
#define MSTD_TEST_STATUS_LIST(X) \
    X(Ok, 0)                     \
    X(Warning, 10)               \
    X(Error, 20)                 \
    X(Critical)

    MSTD_ENUM(Status, unsigned, MSTD_TEST_STATUS_LIST)

    // An enum whose names collide only by case must still be usable via the
    // case-sensitive API even though from_stringCaseInsensitive would be
    // ill-formed for it; keep this one collision-free and dedicated to
    // checking that enums nested in namespaces work identically.
    namespace nested
    {
#define MSTD_TEST_DIRECTION_LIST(X) \
    X(North)                        \
    X(East)                         \
    X(South)                        \
    X(West)

        MSTD_ENUM(Direction, unsigned char, MSTD_TEST_DIRECTION_LIST)
    }   // namespace nested

    // Power-of-two valued bitflag enum, exercising MSTD_ENUM_BITFLAG on top
    // of everything MSTD_ENUM already provides. A byte-sized underlying type
    // keeps the operator~ results easy to reason about in assertions.
#define MSTD_TEST_PERMISSION_LIST(X) \
    X(None, 0)                       \
    X(Read, 1)                       \
    X(Write, 2)                      \
    X(Execute, 4)

    MSTD_ENUM_BITFLAG(Permission, std::uint8_t, MSTD_TEST_PERMISSION_LIST)
}   // namespace

TEST_CASE("MSTD_ENUM assigns sequential implicit values", "[enum]")
{
    STATIC_REQUIRE(static_cast<int>(Color::Red) == 0);
    STATIC_REQUIRE(static_cast<int>(Color::Green) == 1);
    STATIC_REQUIRE(static_cast<int>(Color::Blue) == 2);
}

TEST_CASE(
    "MSTD_ENUM honours explicit values and resumes sequentially after them",
    "[enum]"
)
{
    STATIC_REQUIRE(static_cast<unsigned>(Status::Ok) == 0);
    STATIC_REQUIRE(static_cast<unsigned>(Status::Warning) == 10);
    STATIC_REQUIRE(static_cast<unsigned>(Status::Error) == 20);
    STATIC_REQUIRE(static_cast<unsigned>(Status::Critical) == 21);
}

TEST_CASE(
    "MSTD_ENUM produces an enum class with the requested underlying type",
    "[enum]"
)
{
    STATIC_REQUIRE(std::is_same_v<std::underlying_type_t<Color>, int>);
    STATIC_REQUIRE(std::is_same_v<std::underlying_type_t<Status>, unsigned>);
    STATIC_REQUIRE(std::is_enum_v<Color>);
    STATIC_REQUIRE_FALSE(std::is_convertible_v<Color, int>);
    STATIC_REQUIRE(std::is_enum_v<Status>);
    STATIC_REQUIRE_FALSE(std::is_convertible_v<Status, unsigned>);
}

TEST_CASE(
    "Meta::type and Meta::underlying_type alias the enum and its underlying "
    "type",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(std::is_same_v<ColorMeta::type, Color>);
    STATIC_REQUIRE(std::is_same_v<ColorMeta::underlying_type, int>);
    STATIC_REQUIRE(std::is_same_v<StatusMeta::type, Status>);
    STATIC_REQUIRE(std::is_same_v<StatusMeta::underlying_type, unsigned>);
}

TEST_CASE("Meta::EnumNameStr holds the stringified enum name", "[enum][meta]")
{
    STATIC_REQUIRE(ColorMeta::EnumNameStr == "Color");
    STATIC_REQUIRE(StatusMeta::EnumNameStr == "Status");
    STATIC_REQUIRE(nested::DirectionMeta::EnumNameStr == "Direction");
}

TEST_CASE("Meta::size matches the number of enumerators", "[enum][meta]")
{
    STATIC_REQUIRE(ColorMeta::size == 3);
    STATIC_REQUIRE(StatusMeta::size == 4);
    STATIC_REQUIRE(nested::DirectionMeta::size == 4);
}

TEST_CASE(
    "Meta::values lists every enumerator in declaration order",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(ColorMeta::values.size() == 3);
    STATIC_REQUIRE(ColorMeta::values[0] == Color::Red);
    STATIC_REQUIRE(ColorMeta::values[1] == Color::Green);
    STATIC_REQUIRE(ColorMeta::values[2] == Color::Blue);

    STATIC_REQUIRE(StatusMeta::values.size() == 4);
    STATIC_REQUIRE(StatusMeta::values[0] == Status::Ok);
    STATIC_REQUIRE(StatusMeta::values[1] == Status::Warning);
    STATIC_REQUIRE(StatusMeta::values[2] == Status::Error);
    STATIC_REQUIRE(StatusMeta::values[3] == Status::Critical);
}

TEST_CASE(
    "Meta::values_view exposes the values array as a span",
    "[enum][meta]"
)
{
    constexpr auto view = ColorMeta::values_view();
    STATIC_REQUIRE(view.size() == ColorMeta::values.size());
    STATIC_REQUIRE(view.data() == ColorMeta::values.data());

    REQUIRE(view.size() == 3);
    REQUIRE(view[0] == Color::Red);
    REQUIRE(view[1] == Color::Green);
    REQUIRE(view[2] == Color::Blue);
}

TEST_CASE(
    "Meta::names lists every enumerator name in declaration order",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(ColorMeta::names().size() == 3);
    STATIC_REQUIRE(ColorMeta::names()[0] == "Red");
    STATIC_REQUIRE(ColorMeta::names()[1] == "Green");
    STATIC_REQUIRE(ColorMeta::names()[2] == "Blue");

    STATIC_REQUIRE(StatusMeta::names().size() == 4);
    STATIC_REQUIRE(StatusMeta::names()[0] == "Ok");
    STATIC_REQUIRE(StatusMeta::names()[1] == "Warning");
    STATIC_REQUIRE(StatusMeta::names()[2] == "Error");
    STATIC_REQUIRE(StatusMeta::names()[3] == "Critical");
}

TEST_CASE(
    "Meta::begin/end iterate over values in declaration order",
    "[enum][meta]"
)
{
    std::vector<Color> collected;
    for (auto it = ColorMeta::begin(); it != ColorMeta::end(); ++it)
        collected.push_back(*it);

    REQUIRE(
        collected == std::vector<Color>{Color::Red, Color::Green, Color::Blue}
    );

    // Also confirm the class is usable with range-based for via begin()/end().
    const auto count = std::distance(
        std::begin(ColorMeta::values),
        std::end(ColorMeta::values)
    );
    REQUIRE(count == 3);
}

TEST_CASE("Meta::name resolves every declared enumerator", "[enum][meta]")
{
    STATIC_REQUIRE(ColorMeta::name(Color::Red) == "Red");
    STATIC_REQUIRE(ColorMeta::name(Color::Green) == "Green");
    STATIC_REQUIRE(ColorMeta::name(Color::Blue) == "Blue");

    STATIC_REQUIRE(StatusMeta::name(Status::Warning) == "Warning");
    STATIC_REQUIRE(StatusMeta::name(Status::Critical) == "Critical");
}

TEST_CASE(
    "Meta::name returns an empty string_view for an out-of-range value",
    "[enum][meta]"
)
{
    constexpr auto bogus = static_cast<Color>(99);
    STATIC_REQUIRE(ColorMeta::name(bogus) == std::string_view{});
    STATIC_REQUIRE(ColorMeta::name(bogus).empty());
}

TEST_CASE(
    "Meta::toString mirrors Meta::name but returns std::string",
    "[enum][meta]"
)
{
    REQUIRE(ColorMeta::toString(Color::Red) == std::string("Red"));
    REQUIRE(ColorMeta::toString(Color::Blue) == std::string("Blue"));
    REQUIRE(StatusMeta::toString(Status::Error) == std::string("Error"));

    constexpr auto bogus = static_cast<Status>(255);
    REQUIRE(StatusMeta::toString(bogus).empty());
}

TEST_CASE(
    "Meta::from_string parses exact, case-sensitive enumerator names",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(ColorMeta::from_string("Red") == Color::Red);
    STATIC_REQUIRE(ColorMeta::from_string("Green") == Color::Green);
    STATIC_REQUIRE(ColorMeta::from_string("Blue") == Color::Blue);

    STATIC_REQUIRE(StatusMeta::from_string("Critical") == Status::Critical);
}

TEST_CASE(
    "Meta::from_string rejects unknown, mismatched-case, or empty input",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(ColorMeta::from_string("Purple") == std::nullopt);
    STATIC_REQUIRE(ColorMeta::from_string("red") == std::nullopt);
    STATIC_REQUIRE(ColorMeta::from_string("") == std::nullopt);
    STATIC_REQUIRE(ColorMeta::from_string("Red ") == std::nullopt);
}

TEST_CASE(
    "Meta::from_stringCaseInsensitive matches regardless of case",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(ColorMeta::from_stringCaseInsensitive("red") == Color::Red);
    STATIC_REQUIRE(ColorMeta::from_stringCaseInsensitive("RED") == Color::Red);
    STATIC_REQUIRE(ColorMeta::from_stringCaseInsensitive("ReD") == Color::Red);
    STATIC_REQUIRE(
        ColorMeta::from_stringCaseInsensitive("Blue") == Color::Blue
    );

    STATIC_REQUIRE(
        nested::DirectionMeta::from_stringCaseInsensitive("north") ==
        nested::Direction::North
    );
}

TEST_CASE(
    "Meta::from_stringCaseInsensitive rejects unknown input",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(
        ColorMeta::from_stringCaseInsensitive("purple") == std::nullopt
    );
    STATIC_REQUIRE(ColorMeta::from_stringCaseInsensitive("") == std::nullopt);
}

TEST_CASE(
    "Meta::to_underlying returns the exact underlying representation",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(ColorMeta::to_underlying(Color::Red) == 0);
    STATIC_REQUIRE(ColorMeta::to_underlying(Color::Green) == 1);
    STATIC_REQUIRE(ColorMeta::to_underlying(Color::Blue) == 2);

    STATIC_REQUIRE(StatusMeta::to_underlying(Status::Ok) == 0U);
    STATIC_REQUIRE(StatusMeta::to_underlying(Status::Warning) == 10U);
    STATIC_REQUIRE(StatusMeta::to_underlying(Status::Error) == 20U);
    STATIC_REQUIRE(StatusMeta::to_underlying(Status::Critical) == 21U);

    STATIC_REQUIRE(
        std::is_same_v<decltype(ColorMeta::to_underlying(Color::Red)), int>
    );
}

TEST_CASE(
    "Meta::index returns the declaration-order position of a value",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(ColorMeta::index(Color::Red) == 0);
    STATIC_REQUIRE(ColorMeta::index(Color::Green) == 1);
    STATIC_REQUIRE(ColorMeta::index(Color::Blue) == 2);

    STATIC_REQUIRE(StatusMeta::index(Status::Ok) == 0);
    STATIC_REQUIRE(StatusMeta::index(Status::Critical) == 3);
}

TEST_CASE(
    "Meta::index returns nullopt for an out-of-range value",
    "[enum][meta]"
)
{
    constexpr auto bogus = static_cast<Color>(-1);
    STATIC_REQUIRE(ColorMeta::index(bogus) == std::nullopt);
}

TEST_CASE(
    "enum_meta(EnumName) is discoverable via ADL and returns the Meta type",
    "[enum][meta]"
)
{
    STATIC_REQUIRE(std::is_same_v<decltype(enum_meta(Color{})), ColorMeta>);
    STATIC_REQUIRE(std::is_same_v<decltype(enum_meta(Status{})), StatusMeta>);
    STATIC_REQUIRE(
        std::is_same_v<
            decltype(enum_meta(nested::Direction{})),
            nested::DirectionMeta>
    );
}

TEST_CASE(
    "mstd::enum_meta_t and mstd::has_enum_meta reflect MSTD_ENUM-generated "
    "types",
    "[enum][traits]"
)
{
    STATIC_REQUIRE(std::is_same_v<mstd::enum_meta_t<Color>, ColorMeta>);
    STATIC_REQUIRE(std::is_same_v<mstd::enum_meta_t<Status>, StatusMeta>);

    STATIC_REQUIRE(mstd::has_enum_meta<Color>);
    STATIC_REQUIRE(mstd::has_enum_meta<Status>);
    STATIC_REQUIRE(mstd::has_enum_meta<nested::Direction>);
}

namespace
{
    // A plain enum, not declared via MSTD_ENUM, used to confirm that
    // has_enum_meta correctly reports false for types without generated
    // metadata.
    enum class PlainEnum
    {
        A,
        B
    };
}   // namespace

TEST_CASE(
    "mstd::has_enum_meta is false for types without generated metadata",
    "[enum][traits]"
)
{
    STATIC_REQUIRE_FALSE(mstd::has_enum_meta<PlainEnum>);
    STATIC_REQUIRE_FALSE(mstd::has_enum_meta<int>);
}

TEST_CASE(
    "Round-tripping every enumerator through name/from_string is stable",
    "[enum][meta]"
)
{
    for (const auto value : ColorMeta::values)
    {
        const auto name = ColorMeta::name(value);
        REQUIRE_FALSE(name.empty());
        REQUIRE(ColorMeta::from_string(name) == value);
        REQUIRE(ColorMeta::from_stringCaseInsensitive(name) == value);
        REQUIRE(ColorMeta::to_underlying(value) == static_cast<int>(value));
    }

    for (const auto value : StatusMeta::values)
    {
        const auto name = StatusMeta::name(value);
        REQUIRE_FALSE(name.empty());
        REQUIRE(StatusMeta::from_string(name) == value);
        const auto idx = StatusMeta::index(value);
        REQUIRE(idx.has_value());
        REQUIRE(StatusMeta::values[*idx] == value);
    }
}

// -----------------------------------------------------------------------
// MSTD_ENUM_BITFLAG
// -----------------------------------------------------------------------

TEST_CASE(
    "MSTD_ENUM_BITFLAG still generates the full MSTD_ENUM metadata",
    "[enum][bitflag]"
)
{
    STATIC_REQUIRE(std::is_same_v<PermissionMeta::type, Permission>);
    STATIC_REQUIRE(
        std::is_same_v<PermissionMeta::underlying_type, std::uint8_t>
    );
    STATIC_REQUIRE(PermissionMeta::size == 4);
    STATIC_REQUIRE(PermissionMeta::name(Permission::Read) == "Read");
    STATIC_REQUIRE(
        PermissionMeta::from_string("Execute") == Permission::Execute
    );
    STATIC_REQUIRE(PermissionMeta::to_underlying(Permission::Write) == 2U);
}

TEST_CASE(
    "operator| combines bitflags via a bitwise OR of the underlying value",
    "[enum][bitflag]"
)
{
    constexpr auto combined = Permission::Read | Permission::Write;
    STATIC_REQUIRE(PermissionMeta::to_underlying(combined) == 0b011);

    constexpr auto all =
        Permission::Read | Permission::Write | Permission::Execute;
    STATIC_REQUIRE(PermissionMeta::to_underlying(all) == 0b111);

    // Combining with None is a no-op.
    STATIC_REQUIRE((Permission::Read | Permission::None) == Permission::Read);
}

TEST_CASE("operator|= mutates the left-hand side in place", "[enum][bitflag]")
{
    auto flags  = Permission::Read;
    flags      |= Permission::Write;
    REQUIRE(PermissionMeta::to_underlying(flags) == 0b011);

    flags |= Permission::Execute;
    REQUIRE(PermissionMeta::to_underlying(flags) == 0b111);

    // Re-applying an already-set bit changes nothing.
    flags |= Permission::Read;
    REQUIRE(PermissionMeta::to_underlying(flags) == 0b111);
}

TEST_CASE(
    "operator~ inverts every bit of the underlying representation",
    "[enum][bitflag]"
)
{
    STATIC_REQUIRE(PermissionMeta::to_underlying(~Permission::None) == 0xFF);
    STATIC_REQUIRE(PermissionMeta::to_underlying(~Permission::Read) == 0xFE);
    STATIC_REQUIRE(PermissionMeta::to_underlying(~(~Permission::Read)) == 0x01);
}

TEST_CASE(
    "operator& yields a FlagTest that reports whether any bit overlaps",
    "[enum][bitflag]"
)
{
    constexpr auto readWrite = Permission::Read | Permission::Write;

    STATIC_REQUIRE(static_cast<bool>(readWrite & Permission::Read));
    STATIC_REQUIRE(static_cast<bool>(readWrite & Permission::Write));
    STATIC_REQUIRE_FALSE(static_cast<bool>(readWrite & Permission::Execute));
    STATIC_REQUIRE_FALSE(static_cast<bool>(readWrite & Permission::None));

    if (readWrite & Permission::Read)
        SUCCEED("FlagTest is usable directly in a boolean context");
    else
        FAIL("expected the Read bit to be set");
}

TEST_CASE(
    "operator& converts back to EnumName, exposing the shared bits",
    "[enum][bitflag]"
)
{
    constexpr auto       readWrite = Permission::Read | Permission::Write;
    constexpr Permission shared =
        readWrite & (Permission::Write | Permission::Execute);

    STATIC_REQUIRE(shared == Permission::Write);
    STATIC_REQUIRE(PermissionMeta::to_underlying(shared) == 0b010);
}

TEST_CASE(
    "operator&= narrows the left-hand side to the intersection of flags",
    "[enum][bitflag]"
)
{
    auto flags  = Permission::Read | Permission::Write | Permission::Execute;
    flags      &= (Permission::Write | Permission::Execute);
    REQUIRE(PermissionMeta::to_underlying(flags) == 0b110);

    flags &= Permission::None;
    REQUIRE(PermissionMeta::to_underlying(flags) == 0);
}

TEST_CASE(
    "operator! reports whether an enum value has no bits set",
    "[enum][bitflag]"
)
{
    STATIC_REQUIRE(!Permission::None);
    STATIC_REQUIRE_FALSE(!Permission::Read);
    STATIC_REQUIRE_FALSE(!(Permission::Read | Permission::Write));
}

TEST_CASE(
    "Bitflag operators compose to implement a typical has-flag check",
    "[enum][bitflag]"
)
{
    constexpr auto granted = Permission::Read | Permission::Execute;

    auto hasFlag = [](Permission value, Permission flag)
    { return static_cast<bool>(value & flag); };

    REQUIRE(hasFlag(granted, Permission::Read));
    REQUIRE(hasFlag(granted, Permission::Execute));
    REQUIRE_FALSE(hasFlag(granted, Permission::Write));
    REQUIRE_FALSE(hasFlag(granted, Permission::None));
}

// -----------------------------------------------------------------------
// mstd::EnumAliases / mstd::EnumNames
//
// These customization points are specialized for an enum in the SAME
// header, directly after its MSTD_ENUM(...) declaration -- see
// mstd/enum_extras.hpp for the full contract and the compile-time clash
// checks (mstd::makeAliases / mstd::makeNames) that guard them.
// -----------------------------------------------------------------------

namespace
{
    // An enum with BOTH an alias table and a name override, so the two
    // customization points can be exercised together as well as in
    // isolation via the enums below.
#define MSTD_TEST_VIRIAL_LIST(X) \
    X(Atomic)                    \
    X(Molecular)

    MSTD_ENUM(VirialType, int, MSTD_TEST_VIRIAL_LIST)
}   // namespace

template <>
struct mstd::EnumAliases<VirialType>
{
    static constexpr auto value = mstd::makeAliases<VirialType>(
        {{"mol", VirialType::Molecular}, {"atom", VirialType::Atomic}}
    );
};

template <>
struct mstd::EnumNames<VirialType>
{
    static constexpr auto value =
        mstd::makeNames<VirialType>({{VirialType::Molecular, "MolecularVirial"}}
        );
};

namespace
{
    // Aliases only -- no EnumNames specialization at all.
#define MSTD_TEST_MEASURE_LIST(X) \
    X(Meter)                      \
    X(Second)

    MSTD_ENUM(MeasureType, int, MSTD_TEST_MEASURE_LIST)
}   // namespace

template <>
struct mstd::EnumAliases<MeasureType>
{
    static constexpr auto value = mstd::makeAliases<MeasureType>(
        {{"m", MeasureType::Meter},
         {"metre", MeasureType::Meter},
         {"s", MeasureType::Second}}
    );
};

namespace
{
    // A name override only -- no EnumAliases specialization at all.
#define MSTD_TEST_LEVEL_LIST(X) \
    X(Low)                      \
    X(Medium)                   \
    X(High)

    MSTD_ENUM(LevelType, int, MSTD_TEST_LEVEL_LIST)
}   // namespace

template <>
struct mstd::EnumNames<LevelType>
{
    static constexpr auto value =
        mstd::makeNames<LevelType>({{LevelType::Medium, "Mid"}});
};

namespace
{
    // Neither customization point specialized -- the default, empty tables
    // apply; behaviour must be identical to a plain MSTD_ENUM enum.
#define MSTD_TEST_SHAKE_LIST(X) \
    X(None)                     \
    X(Shake)                    \
    X(Rattle)

    MSTD_ENUM(ShakeType, int, MSTD_TEST_SHAKE_LIST)
}   // namespace

TEST_CASE(
    "Meta::originalNames holds the raw, macro-generated spelling for every "
    "enumerator, regardless of any EnumNames override",
    "[enum][alias][meta]"
)
{
    STATIC_REQUIRE(VirialTypeMeta::originalNames[0] == "Atomic");
    STATIC_REQUIRE(VirialTypeMeta::originalNames[1] == "Molecular");
}

TEST_CASE(
    "Meta::names() equals Meta::originalNames when no EnumNames override is "
    "registered for that enumerator",
    "[enum][alias][meta]"
)
{
    STATIC_REQUIRE(
        VirialTypeMeta::names()[0] == VirialTypeMeta::originalNames[0]
    );
    STATIC_REQUIRE(
        ShakeTypeMeta::names()[0] == ShakeTypeMeta::originalNames[0]
    );
    STATIC_REQUIRE(
        ShakeTypeMeta::names()[1] == ShakeTypeMeta::originalNames[1]
    );
    STATIC_REQUIRE(
        ShakeTypeMeta::names()[2] == ShakeTypeMeta::originalNames[2]
    );
}

TEST_CASE(
    "Meta::names() reflects an EnumNames override for the overridden "
    "enumerator only, leaving the rest untouched",
    "[enum][alias][meta]"
)
{
    STATIC_REQUIRE(VirialTypeMeta::names()[0] == "Atomic");   // untouched
    STATIC_REQUIRE(
        VirialTypeMeta::names()[1] == "MolecularVirial"
    );   // overridden
}

TEST_CASE(
    "Meta::name()/Meta::toString() use the EnumNames override when one is "
    "registered, and fall back to the generated name otherwise",
    "[enum][alias][meta]"
)
{
    STATIC_REQUIRE(VirialTypeMeta::name(VirialType::Atomic) == "Atomic");
    STATIC_REQUIRE(
        VirialTypeMeta::name(VirialType::Molecular) == "MolecularVirial"
    );

    REQUIRE(
        VirialTypeMeta::toString(VirialType::Molecular) ==
        std::string("MolecularVirial")
    );
}

TEST_CASE(
    "Meta::from_string accepts the original name, the overridden name, and "
    "an alias, all exact-case",
    "[enum][alias][meta]"
)
{
    STATIC_REQUIRE(
        VirialTypeMeta::from_string("Molecular") == VirialType::Molecular
    );
    STATIC_REQUIRE(
        VirialTypeMeta::from_string("MolecularVirial") == VirialType::Molecular
    );
    STATIC_REQUIRE(VirialTypeMeta::from_string("mol") == VirialType::Molecular);
    STATIC_REQUIRE(VirialTypeMeta::from_string("Atomic") == VirialType::Atomic);
}

TEST_CASE(
    "Meta::from_string stays exact-case for overridden names and aliases, "
    "just as it does for generated names",
    "[enum][alias][meta]"
)
{
    STATIC_REQUIRE(
        VirialTypeMeta::from_string("molecularvirial") == std::nullopt
    );
    STATIC_REQUIRE(VirialTypeMeta::from_string("MOL") == std::nullopt);
}

TEST_CASE(
    "Meta::from_stringCaseInsensitive accepts the original name, the "
    "overridden name, and an alias, regardless of case",
    "[enum][alias][meta]"
)
{
    STATIC_REQUIRE(
        VirialTypeMeta::from_stringCaseInsensitive("molecular") ==
        VirialType::Molecular
    );
    STATIC_REQUIRE(
        VirialTypeMeta::from_stringCaseInsensitive("MOLECULARVIRIAL") ==
        VirialType::Molecular
    );
    STATIC_REQUIRE(
        VirialTypeMeta::from_stringCaseInsensitive("MOL") ==
        VirialType::Molecular
    );
    STATIC_REQUIRE(
        VirialTypeMeta::from_stringCaseInsensitive("ATOM") == VirialType::Atomic
    );
}

TEST_CASE(
    "an enum with no EnumAliases/EnumNames specialization behaves exactly "
    "like a plain MSTD_ENUM enum",
    "[enum][alias][no_extras]"
)
{
    STATIC_REQUIRE(ShakeTypeMeta::from_string("Shake") == ShakeType::Shake);
    STATIC_REQUIRE(
        ShakeTypeMeta::from_stringCaseInsensitive("rattle") == ShakeType::Rattle
    );
    STATIC_REQUIRE(ShakeTypeMeta::from_string("shake") == std::nullopt);
}

TEST_CASE(
    "an enum may register aliases without any EnumNames override",
    "[enum][alias][only]"
)
{
    // names()/name()/toString() are completely unaffected -- no override
    STATIC_REQUIRE(MeasureTypeMeta::names()[0] == "Meter");
    STATIC_REQUIRE(MeasureTypeMeta::name(MeasureType::Meter) == "Meter");

    // but from_string/from_stringCaseInsensitive accept every alias, plus
    // the original spelling
    STATIC_REQUIRE(MeasureTypeMeta::from_string("m") == MeasureType::Meter);
    STATIC_REQUIRE(MeasureTypeMeta::from_string("metre") == MeasureType::Meter);
    STATIC_REQUIRE(MeasureTypeMeta::from_string("s") == MeasureType::Second);
    STATIC_REQUIRE(MeasureTypeMeta::from_string("Meter") == MeasureType::Meter);
}

TEST_CASE(
    "multiple aliases may resolve to the same enumerator value",
    "[enum][alias][multiple]"
)
{
    STATIC_REQUIRE(MeasureTypeMeta::from_string("m") == MeasureType::Meter);
    STATIC_REQUIRE(MeasureTypeMeta::from_string("metre") == MeasureType::Meter);
    STATIC_REQUIRE(
        MeasureTypeMeta::from_string("m") ==
        MeasureTypeMeta::from_string("metre")
    );
}

TEST_CASE(
    "an enum may register an EnumNames override without any aliases",
    "[enum][override][only]"
)
{
    // the override changes the display name for MEDIUM only...
    STATIC_REQUIRE(LevelTypeMeta::names()[1] == "Mid");
    STATIC_REQUIRE(LevelTypeMeta::name(LevelType::Medium) == "Mid");
    // ...untouched enumerators keep their generated name
    STATIC_REQUIRE(LevelTypeMeta::name(LevelType::Low) == "Low");
    STATIC_REQUIRE(LevelTypeMeta::name(LevelType::High) == "High");

    // from_string accepts both the original and the overridden spelling
    STATIC_REQUIRE(LevelTypeMeta::from_string("Medium") == LevelType::Medium);
    STATIC_REQUIRE(LevelTypeMeta::from_string("Mid") == LevelType::Medium);
    STATIC_REQUIRE(LevelTypeMeta::from_string("mid") == std::nullopt);
    STATIC_REQUIRE(
        LevelTypeMeta::from_stringCaseInsensitive("mid") == LevelType::Medium
    );
}

namespace
{
    // Dedicated enum for the "override re-cases its own name" check: FOO's
    // override is a pure case change of its own original name, nothing else.
#define MSTD_TEST_RECASE_LIST(X) \
    X(FOO)                       \
    X(BAR)

    MSTD_ENUM(RecaseType, int, MSTD_TEST_RECASE_LIST)
}   // namespace

template <>
struct mstd::EnumNames<RecaseType>
{
    static constexpr auto value = mstd::makeNames<RecaseType>(
        {{RecaseType::FOO, "Foo"}}   // re-cases its OWN original name
    );
};

TEST_CASE(
    "an EnumNames override that only re-cases its own original name is not "
    "flagged as a clash",
    "[enum][override][self_recase]"
)
{
    using M = RecaseTypeMeta;

    STATIC_REQUIRE(M::originalNames[0] == "FOO");
    STATIC_REQUIRE(M::name(RecaseType::FOO) == "Foo");
    STATIC_REQUIRE(M::from_stringCaseInsensitive("foo") == RecaseType::FOO);
    STATIC_REQUIRE(
        M::from_string("FOO") == RecaseType::FOO
    );   // original spelling still parses too
}

TEST_CASE(
    "an alias and an EnumNames override may coexist for different "
    "enumerators of the same enum without interfering with each other",
    "[enum][alias][override][coexist]"
)
{
    // Atomic has only an alias ("atom"), no override -- its generated name
    // is untouched.
    STATIC_REQUIRE(VirialTypeMeta::name(VirialType::Atomic) == "Atomic");
    STATIC_REQUIRE(VirialTypeMeta::from_string("atom") == VirialType::Atomic);

    // Molecular has both an alias ("mol") and an override
    // ("MolecularVirial") -- all three spellings resolve to it.
    STATIC_REQUIRE(
        VirialTypeMeta::from_string("Molecular") == VirialType::Molecular
    );
    STATIC_REQUIRE(
        VirialTypeMeta::from_string("MolecularVirial") == VirialType::Molecular
    );
    STATIC_REQUIRE(VirialTypeMeta::from_string("mol") == VirialType::Molecular);
}

// -----------------------------------------------------------------------
// Tables built only via mstd::makeAliases/mstd::makeNames are checked --
// see compile_fail/raw_array_instead_of_make_aliases.cpp,
// compile_fail/raw_array_instead_of_make_names.cpp, and
// compile_fail/names_alone_catches_alias_clash.cpp for the corresponding
// negative (must NOT compile) cases.
// -----------------------------------------------------------------------

TEST_CASE(
    "EnumAliases<E>::value and EnumNames<E>::value built via "
    "mstd::makeAliases/mstd::makeNames satisfy the checked-table "
    "requirement, so every accessor -- including names() alone -- works",
    "[enum][alias][override][checked_table]"
)
{
    // VirialType's tables (declared above) both go through
    // makeAliases/makeNames; names() alone touching them must be fine.
    STATIC_REQUIRE(VirialTypeMeta::names()[1] == "MolecularVirial");
    STATIC_REQUIRE(VirialTypeMeta::names().size() == VirialTypeMeta::size);
}
