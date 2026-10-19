#ifndef __MSTD__UNITS_HPP__
#define __MSTD__UNITS_HPP__

#include <mp-units/systems/si.h>
#include <mp-units/systems/si/math.h>   // adjust to where your grep found sin/cos

#include "mstd/math.hpp"   // needed for mstd::detail::adl_tag

namespace mstd::detail
{
    // quantity overloads: more specialized than the generic fallback, so they
    // win
#define MSTD_UNITS_TRIG_HOOK(fn)                                             \
    template <auto R, typename Rep>                                          \
    constexpr auto fn##_impl(const ::mp_units::quantity<R, Rep> &q, adl_tag) \
    {                                                                        \
        return ::mp_units::si::fn(q).numerical_value_in(::mp_units::one);    \
    }

    MSTD_UNITS_TRIG_HOOK(sin)
    MSTD_UNITS_TRIG_HOOK(cos)
    MSTD_UNITS_TRIG_HOOK(tan)
#undef MSTD_UNITS_TRIG_HOOK
}   // namespace mstd::detail

namespace mstd::units
{
    using namespace mp_units;
    using namespace mp_units::isq;
    using namespace mp_units::si::unit_symbols;
}   // namespace mstd::units

namespace mstd
{
    using mp_units::quantity;
}   // namespace mstd

#endif   // __MSTD__UNITS_HPP__
