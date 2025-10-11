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

#ifndef __MSTD_MATH_POWER_HPP__
#define __MSTD_MATH_POWER_HPP__

namespace mstd
{

    template <typename T>
    T power(T base, int exponent)
    {
        T result = static_cast<T>(1);

        if (exponent < 0)
        {
            base     = 1 / base;
            exponent = -exponent;
        }

        for (int i = 0; i < exponent; i++)
            result *= base;

        return result;
    }

}   // namespace mstd

#endif   // __MSTD_MATH_POWER_HPP__