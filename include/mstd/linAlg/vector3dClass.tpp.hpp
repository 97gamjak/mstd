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

#ifndef __MSTD__LINALG__VECTOR3DCLASS_TPP_HPP__
#define __MSTD__LINALG__VECTOR3DCLASS_TPP_HPP__

#include "vector3dClass.hpp"

namespace mstd
{
    /************************
     *                      *
     * assignment operators *
     *                      *
     ************************/

    /*******************
     * copy assignment *
     *******************/

    template <typename T>
    constexpr Vector3d<T> &Vector3d<T>::operator=(Vector3d<T> &rhs)
    {
        _xyz = rhs._xyz;
        return *this;
    }

    template <typename T>
    constexpr Vector3d<T> &Vector3d<T>::operator=(const Vector3d<T> &rhs)
    {
        _xyz = rhs._xyz;
        return *this;
    }

    /**********************
     *                    *
     * indexing operators *
     *                    *
     **********************/

    template <typename T>
    constexpr T &Vector3d<T>::operator[](const size_t index)
    {
        return _xyz[index];
    }

    template <typename T>
    constexpr const T &Vector3d<T>::operator[](const size_t index) const
    {
        return _xyz[index];
    }

}   // namespace mstd

#endif   //  __MSTD__LINALG__VECTOR3DCLASS_TPP_HPP__
