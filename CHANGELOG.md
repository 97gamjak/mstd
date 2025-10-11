# Changelog

All notable changes to this project will be documented in this file.

## Next Release

### Feature

#### Units

- add velocity unit Angstrom per seconds
- add force unit kcal per mole Angstrom

#### Quantities

- add possibility to multiply and divide more than two unit with one call (see unit_mul, unit_div)

#### Type Traits

- introducing new type_traits library
  - is_unit_v
  - concept UnitType

### Breaking Changes

- all pre-defined units with a division like `mps` are not used like `m_per_s`
- remove all `sub-namespaces` besides `mstd` and `details`  

<!-- insertion marker -->
## [0.0.1](https://github.com/97gamjak/mstd/releases/tag/0.0.1) - 2025-10-04

### Feature

- Introduce Quantity class for compile time unit conversions

### CI

- add CI to create automatically a tag and a release on github

### Bug Fix

- add permissions to create tag workflow

