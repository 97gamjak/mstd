# Changelog

All notable changes to this project will be documented in this file.

## Next Release

### MSTD-CHECKS

- Add checks python library first version for all kind of custom static code analysis
- Add "static inline constexpr" as key sequence order cpp rule

<!-- insertion marker -->
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

