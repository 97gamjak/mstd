#ifndef __MSTD__UNITS_HPP__
#define __MSTD__UNITS_HPP__

#include <mp-units/systems/si.h>

namespace mstd::units
{
    using ::mp_units::si::unit_symbols::deg;
    using ::mp_units::si::unit_symbols::rad;
}   // namespace mstd::units

namespace mstd
{
    using mp_units::quantity;
}   // namespace mstd

#endif   // __MSTD__UNITS_HPP__
