#ifndef __MSTD__UNITS__RELAXATION_TIME_HPP__
#define __MSTD__UNITS__RELAXATION_TIME_HPP__

#include <mp-units/systems/si.h>

namespace mstd::units
{
    /**
     * @brief Specifies the relaxation time quantity in the SI system.
     */
    inline constexpr struct relaxation_time final
        : mp_units::quantity_spec<mp_units::isq::duration>
    {
    } relaxation_time;

}   // namespace mstd::units

#endif   // __MSTD__UNITS__RELAXATION_TIME_HPP__
