#ifndef __MSTD__UNITS__WAVE_NUMBER_HPP__
#define __MSTD__UNITS__WAVE_NUMBER_HPP__

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

#include "mp-units/systems/isq/electromagnetism.h"
#include "namespace.hpp"   // IWYU pragma: keep -- for mstd namespace usage

namespace mstd
{
    /**
     * @brief Converts a wave number to its corresponding frequency.
     *
     * This function takes a wave number and returns the equivalent frequency
     * using the speed of light in vacuum.
     *
     * @param waveNumber The wave number to convert.
     * @return The frequency corresponding to the given wave number in Hz.
     */
    constexpr auto toFreq(units::QuantityOf<units::wavenumber> auto waveNumber)
    {
        return units::quantity_cast<units::frequency>(
                   waveNumber * units::speed_of_light_in_vacuum
        )
            .in(units::Hz);
    }

    /**
     * @brief Converts a frequency to its corresponding wave number.
     *
     * This function takes a frequency and returns the equivalent wave number
     * using the speed of light in vacuum.
     *
     * @param frequency The frequency to convert.
     * @return The wave number corresponding to the given frequency in cm^-1.
     */
    constexpr auto toWavenumber(
        units::QuantityOf<units::frequency> auto frequency
    )
    {
        return units::quantity_cast<units::wavenumber>(
                   frequency / units::speed_of_light_in_vacuum
        )
            .in(units::one / units::cm);
    }

}   // namespace mstd

#endif   // __MSTD__UNITS__WAVE_NUMBER_HPP__
