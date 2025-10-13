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

#ifndef __MSTD_UNITS_ENUMS_HPP__
#define __MSTD_UNITS_ENUMS_HPP__

#include <cstddef>

#include "mstd/enum.hpp"

namespace mstd
{
    // NOLINTBEGIN

    /**
     * @brief Enumeration of the base dimension IDs.
     *
     * This enum provides identifiers for the seven base SI dimensions:
     * length, mass, time, electric current, temperature, amount of
     * substance, luminous intensity. Each identifier corresponds to
     * its position in the SI dimension pack.
     * Additionally, a `Count` identifier is included to represent the
     * total number of base SI dimensions.
     * @note The order of the dimensions is important and should not be changed.
     *       It must match the order used in the `si` pack definition.
     *       The `Count` value is automatically set to the number of dimensions.
     */

#define SIDIMID_LIST(X) \
    X(Length, 0)        \
    X(Mass)             \
    X(Time)             \
    X(Current)          \
    X(Temperature)      \
    X(Amount)           \
    X(Luminous)

    MSTD_ENUM(SIDimId, size_t, SIDIMID_LIST)

    /**
     * @brief Enumeration of the extra dimension IDs.
     *
     * This enum provides identifiers for the four extra dimensions:
     * angle, currency, information, count. Each identifier corresponds to
     * its position in the extra dimension pack.
     * @note The order of the dimensions is important and should not be
     * changed. It must match the order used in the `ex` pack definition. The
     * `Count` value is automatically set to the number of dimensions.
     */
#define EXTRADIMID_LIST(X) \
    X(Angle, 0)            \
    X(Currency)            \
    X(Info)                \
    X(Count)

    MSTD_ENUM(ExtraDimId, size_t, EXTRADIMID_LIST)

    // NOLINTEND
}   // namespace mstd

#endif   // __MSTD_UNITS_ENUMS_HPP__