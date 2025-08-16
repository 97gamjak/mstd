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

#ifndef __MSTD_UNITS_BASE_HPP__
#define __MSTD_UNITS_BASE_HPP__

#include <cstddef>

namespace mstd
{
    enum class BaseUnitEnum : size_t
    {
        Length,
        Mass,
        Time,
        Temperature,
        AmountOfSubstance,
        Current,
        LuminousIntensity,
    };
}   // namespace mstd

#endif   // __MSTD_UNITS_BASE_HPP__