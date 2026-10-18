#ifndef __MSTD__UNITS_HPP__
#define __MSTD__UNITS_HPP__

#include <mp-units/systems/si.h>

namespace mstd::units
{
    using namespace mp_units;
    using namespace mp_units::si;
    using namespace mp_units::isq;
}   // namespace mstd::units

namespace mstd
{
    using mp_units::quantity;
}   // namespace mstd

#endif   // __MSTD__UNITS_HPP__
