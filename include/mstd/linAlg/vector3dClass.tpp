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

#ifndef __MSTD__LINALG__VECTOR3DCLASS_TPP__
#define __MSTD__LINALG__VECTOR3DCLASS_TPP__

#include "vector3dClass.hpp"

namespace mstd
{
    /**********************
     *                    *
     * indexing operators *
     *                    *
     **********************/

    template <typename T>
    constexpr T &Vector3d<T>::operator[](const std::size_t index) noexcept
    {
        return _xyz[index];
    }

    template <typename T>
    constexpr const T &Vector3d<T>::operator[](const std::size_t index
    ) const noexcept
    {
        return _xyz[index];
    }

}   // namespace mstd

#endif   //  __MSTD__LINALG__VECTOR3DCLASS_TPP__
