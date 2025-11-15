# Changelog

All notable changes to this project will be documented in this file.

## Next Release

### Feature

#### Units

- This functionality is at the moment faulty - DO NOT USE IT

#### Quantities

- This functionality is at the moment faulty - DO NOT USE IT

#### Type Traits

- introducing new type_traits library
- introducing type traits for arithmetic operations, i.e. +,-,*,/

### Breaking Changes

- all pre-defined units with a division like `mps` are not used like `m_per_s`
- remove all `sub-namespaces` besides `mstd` and `details`  
- refactoring of how new units are build (cleaning up internal helper traits)
- rename unit to Unit and remove real_unit

<!-- insertion marker -->
## [0.0.1](https://github.com/97gamjak/mstd/releases/tag/0.0.1) - 2025-10-04

### Feature

- Introduce Quantity class for compile time unit conversions

### CI

- add CI to create automatically a tag and a release on github

### Bug Fix

- add permissions to create tag workflow

