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

#ifndef __MSTD__LINALG__CONCEPTS__VECTOR3DDEPTH_HPP__
#define __MSTD__LINALG__CONCEPTS__VECTOR3DDEPTH_HPP__

#include <concepts>

namespace mstd
{
    template <class T>
    class Vector3d;

    /**
     * @brief type trait to determine the depth of a Vector3d
     *
     * @tparam T
     */
    template <class T>
    struct Vector3dDepth
    {
        static constexpr int value = 0;
    };

    /**
     * @brief type trait to determine the depth of a Vector3d
     *
     * @details specialization for Vector3d
     *
     * @tparam T
     */
    template <class T>
    struct Vector3dDepth<Vector3d<T>>
    {
        static constexpr int value = 1 + Vector3dDepth<T>::value;
    };

    /**
     * @brief constexpr variable to check the depth of a Vector3d
     *
     * @tparam T
     */
    template <class T>
    constexpr int Vector3dDepth_v = Vector3dDepth<T>::value;

    /**
     * @brief constexpr variable to check the depth difference of two Vector3ds
     *
     * @tparam T
     * @tparam U
     */
    template <class T, class U>
    constexpr int Vector3dDepthDifference_v =
        Vector3dDepth_v<T> - Vector3dDepth_v<U>;

}   // namespace mstd

#endif   //  __MSTD__LINALG__CONCEPTS__VECTOR3DDEPTH_HPP__
