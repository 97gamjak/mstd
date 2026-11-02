/*****************************************************************************
<GPL_HEADER>

    mstd library
    Copyright (C) 2025-now  Jakob Gamper

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

<GPL_HEADER>
******************************************************************************/

#ifndef __MSTD__UNITS__WAVE_NUMBER_HPP__
#define __MSTD__UNITS__WAVE_NUMBER_HPP__

#include <mp-units/systems/isq.h>
#include <mp-units/systems/si.h>

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
                   waveNumber * mp_units::si::speed_of_light_in_vacuum
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
                   frequency / mp_units::si::speed_of_light_in_vacuum
        )
            .in(units::one / units::cm);
    }

}   // namespace mstd

#endif   // __MSTD__UNITS__WAVE_NUMBER_HPP__
