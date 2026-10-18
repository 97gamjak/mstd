# Changelog

All notable changes to this project will be documented in this file.

## Next Release

<!-- insertion marker -->
## [0.8.0](https://github.com/repo/owner/releases/tag/0.8.0) - 2026-10-18

### Utils

- add `__CONST_GETTER__(name)`, a helper macro that generates a `const`, `noexcept` getter member function returning the matching `_name` data member, by value for small trivially-copyable types (`sizeof(T) <= 2 * sizeof(void*)`) or by `const&` otherwise, as decided by the new `getter_value`/`pass_by_value_v` helpers; the member function's return type is `decltype(auto)` so it actually reflects `getter_value`'s by-value/by-reference choice instead of forcing a `const&` return that would dangle for the by-value case

## [0.7.0](https://github.com/repo/owner/releases/tag/0.7.0) - 2026-10-16

### Types

- add `mstd::Cache<T, Sources...>`, a lazily-computed cache that recomputes its value only when a domain of one of its `Source` sources has advanced since the last computation; add `mstd::BoundCache<T, Sources...>`, a non-copyable, non-movable variant that binds its sources by address at construction and is itself a `Source`
- add `mstd::MemberCache<T, Owner, Members...>`, a cache that lives as a member of an owner class and names its sources as pointers to other data members of that owner (`Property`/`CacheProperty` or other `MemberCache`s), so the owner keeps its defaulted copy/move/assignment; add `mstd::SyncCache<T, Owner, Members...>`, its thread-safe counterpart, where concurrent `get()` calls recompute a stale value exactly once and a hit is lock-free
- add `mstd::GlobalEpoch<Domain>`, a static atomic epoch counter per domain used to detect when a cache's sources have advanced
- add `mstd::Property<T>` and `mstd::CacheProperty<T, Domain>`, the latter tracking a version/epoch of its value so dependent caches can detect changes
- add `mstd::Id<T, Tag>`, `mstd::Stamp<T>` (an `Id`/`Version` pair) and `mstd::Version` (a `StrongSizeT`), plus the `StrongSizeT<Tag>` alias of `StrongType` for ordered, hashable, incrementable, arithmetic size-based strong types

### Debugging

- add `MSTD_HOT_PATH`/`MSTD_HOT_PATH_STRICT` scope macros and `MSTD_ASSERT_NOT_HOT`/`MSTD_ASSERT_NOT_STRICT`, which throw `PerformanceError` when a write/recompute (or, in strict scopes, a cache hit) happens inside a marked hot path; compiled out entirely unless `MSTD_DEBUG` or `MSTD_PERF_CHECKS` is defined

### Error Handling

- add `mstd::BaseException<ExceptionType>` and the `ExceptionType` enum (currently `RuntimeError`) as the foundation for the library's typed exceptions, used by `PerformanceError`

## [0.6.0](https://github.com/repo/owner/releases/tag/0.6.0) - 2026-10-03

### Types

- add `mstd::Ref<T>`, a rebindable, non-null reference wrapper (and `mstd::ConstRef<T>` shorthand for `Ref<const T>`) intended to replace reference data members, which delete copy/move assignment; unlike `std::reference_wrapper` it provides `operator->`/`operator*`, cannot bind to rvalues, and converts implicitly from `Ref<U>` to `Ref<T>` wherever `U&` converts to `T&`

### Enum

- fix `operator&` on `MSTD_ENUM_BITFLAG` enums failing to compile under `-Werror` for some underlying types: the bitwise `&` of two `Underlying` operands promotes to `int`, which brace-initializing `EnumName##FlagTest` from directly triggered `-Werror=narrowing` for small underlying types (e.g. `uint8_t`); routing the result through a plain `Underlying result = ...;` first avoids that without triggering `-Werror=useless-cast` for `unsigned`-width underlying types

## [0.5.1](https://github.com/repo/owner/releases/tag/0.5.1) - 2026-10-03

### String

- relax `join` to accept any `std::ranges::input_range` instead of requiring the custom `joinable_range` concept, and switch the default element-to-string conversion from `std::string(s)` to `std::format("{}", s)`, so `join` now works on ranges of any formattable element type, not just string-like ones

## [0.5.0](https://github.com/repo/owner/releases/tag/0.5.0) - 2026-10-01

### Enum

- add `EnumName##Meta::aliasNames()`, returning just the alias texts registered in `EnumAliases` as a plain `std::array<std::string_view, N>`, for callers that don't need to know which enumerator each alias belongs to (use `aliases()` for that)
- add `EnumName##Meta::spellingNames()`, returning the text of every `spellings()` entry (original names, `EnumNames` overrides, and `EnumAliases` aliases together) as a plain `std::array<std::string_view, N>`, e.g. for building a "possible values are: ..." message; unlike `aliasNames()`, which only lists the extra alias spellings, this also includes the enum's own names

### String

- add an overload of `join` taking a `transform` function, letting each range element be converted to a string on the fly instead of requiring the range to already hold strings

## [0.4.0](https://github.com/repo/owner/releases/tag/0.4.0) - 2026-09-30

### Enum

- split `enum.hpp` into `enum/enum.hpp`, `enum/enum_bit_flags.hpp` (renamed from `enum_bit_flage.hpp`) and `enum/enum_string.hpp`, with `mstd/enum.hpp` now re-exporting all three
- move `EnumFromString`/`EnumToString` into namespace `mstd`, fixing a compile error in `from_string` which referenced `mstd::EnumFromString` on an undefined name
- add extensive unit test coverage for `MSTD_ENUM` and `MSTD_ENUM_BITFLAG`
- rename `EnumToString`/`EnumFromString` to `EnumNames`/`EnumAliases`, letting `EnumNames` override the generated spelling of individual enumerators (via `name()`/`toString()`) and `EnumAliases` register extra strings that `from_string`/`from_stringCaseInsensitive` accept, on top of the generated name; both are checked at compile time for case-insensitive clashes against each other and the generated names (`mstd::makeNames`/`mstd::makeAliases`)
- turn `EnumName##Meta::names` from a static array into a `names()` member function, so it reflects any `EnumNames` override; the untouched, macro-generated spellings remain available as `EnumName##Meta::originalNames`

## [0.3.0](https://github.com/repo/owner/releases/tag/0.3.0) - 2026-09-26

### File

- add new (very thin) type `File` for type safe string handling 

## [0.2.6](https://github.com/repo/owner/releases/tag/0.2.6) - 2026-09-23

### Enum

- add possiblity to use `from_stringCaseInsensitive` if the different enum values allow it (compile time checked)

## [0.2.5](https://github.com/repo/owner/releases/tag/0.2.5) - 2026-09-20

### Enum

- add operator~ and operator&=

## [0.2.4](https://github.com/repo/owner/releases/tag/0.2.4) - 2026-09-16

### Enum

- more clang-tidy fix for enum type

## [0.2.3](https://github.com/repo/owner/releases/tag/0.2.3) - 2026-08-26

### Types

- add `std::hash` function to strong type definition

## [0.2.2](https://github.com/repo/owner/releases/tag/0.2.2) - 2026-08-22

### Enum

- make mstd enum type clang-tidy conform

## [0.2.1](https://github.com/repo/owner/releases/tag/0.2.1) - 2026-08-19

### Types

- as for requires g++ needs static cast to bool for flagset - this bug has been fixed

## [0.2.0](https://github.com/repo/owner/releases/tag/0.2.0) - 2026-08-18

### Types

- add convenient template for creating strong types easily

### Type Traits

- Introduce new concept `HasToString`

## [0.1.5](https://github.com/repo/owner/releases/tag/0.1.5) - 2026-08-13

### Enum

- make enum.hpp clang-tidy compliant

## [0.1.4](https://github.com/repo/owner/releases/tag/0.1.4) - 2026-07-26

### Compilation

- Add version 14.0 as minimum requirement for the gcc compiler

### Feature

- add mp-units library
- add `operator|=` for bitflag enums

## [0.1.3](https://github.com/repo/owner/releases/tag/0.1.3) - 2026-06-13

### Enum

- add `EnumNameStr` as a member of EnumMeta

## [0.1.2](https://github.com/repo/owner/releases/tag/0.1.2) - 2026-05-07

### Enum

- define only forward declaration if MOC_RUN compilation is processing enum definition

## [0.1.1](https://github.com/repo/owner/releases/tag/0.1.1) - 2026-03-09

- Update test/CMakeLists.txt to implicitly take care of including the Catch2 submodule

### Type Traits

- Fix missing concept header in `arithmetic_traits.hpp`

## [0.1.0](https://github.com/repo/owner/releases/tag/0.1.0) - 2026-02-27

### Enum

- introduce `MSTD_ENUM_BITFLAG` for bitwise operations on enums

### String

- introduce new string library
- add `join` function to concatenate string_view like ranges with a delimiter

### Type Traits

- add concept `joinable_range`

## [0.0.3](https://github.com/repo/owner/releases/tag/0.0.3) - 2026-02-08

### CI

- Add devops python library first version for all kind of custom static code analysis
- Add `update_changelog` from devops package to create_tag CI
- Add hotfix branch release handling to auto increment version patch aka `major.minor.patch`

### Error Handling

- Add compile time warning macros for buggy libraries and buggy headers (used atm in quantity lib as long as it is not fixed)

### Compilation

- Add Compile flag `MSTD_IGNORE_BUGGY_CODE` to ignore any kind of warnings for buggy libraries or headers
- Add exhaustive error flags for compilation
- Add checking of doxygen comments via building docs (no docs is building for gh pages - yet)

### Cleanup

- Cleanup header guards to follow common rule with folder and file structure

### Enums

- Introduce type traits for mstd enums: `using EnumMeta = typename enum_meta<Enum>::type`
- Make `EnumMeta::index` return std::optional instead of -1 in case of error
- Add a `values_view` to `EnumMeta` returning a `std::span` for having a nicer API in special cases

## [0.0.2](https://github.com/97gamjak/mstd/releases/tag/0.0.2) - 2025-11-20

### New Feature

#### Physics

- lie potential with general M,N potential (documentation coming soon)
- Lennard Jones potential as specialization of lie potential (documentation coming soon)

#### Math

- `cpow` function for compile time power function with integer exponents 

#### Type Traits

- introducing new type_traits library
- introducing type traits for arithmetic operations, i.e. +,-,*,/
  
### Feature

#### Quantity

- This functionality is at the moment faulty - DO NOT USE IT

### Known Bugs

- full quantity library is not usable at the moment!

### Breaking Changes

- remove all `sub-namespaces` besides `mstd` and `details`  

## [0.0.1](https://github.com/97gamjak/mstd/releases/tag/0.0.1) - 2025-10-04

### Feature

- Introduce Quantity class for compile time unit conversions

### CI

- add CI to create automatically a tag and a release on github

### Bug Fix

- add permissions to create tag workflow






















