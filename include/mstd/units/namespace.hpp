#ifndef __MSTD__UNITS__NAMESPACE_HPP__
#define __MSTD__UNITS__NAMESPACE_HPP__

#include <mp-units/systems/si.h>

namespace mstd::units
{
    using namespace mp_units;
    using namespace mp_units::isq;
    using namespace mp_units::si::unit_symbols;

    inline constexpr auto per_cm = one / cm;
}   // namespace mstd::units

namespace mstd
{
    using mp_units::quantity;
}   // namespace mstd

#endif   // __MSTD__UNITS__NAMESPACE_HPP__
