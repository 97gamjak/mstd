# Changelog

All notable changes to this project will be documented in this file.

## Next Release

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

