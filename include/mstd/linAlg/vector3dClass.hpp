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

#ifndef __MSTD__LINALG__VECTOR3D_CLASS_HPP__
#define __MSTD__LINALG__VECTOR3D_CLASS_HPP__

#include <array>
#include <concepts>
#include <ostream>

namespace mstd
{
    template <typename T>
    class Vector3d
    {
       private:
        std::array<T, 3> _xyz;

       public:
        ~Vector3d() = default;

        constexpr Vector3d() = default;
        constexpr Vector3d(const T x, const T y, const T z) : _xyz{x, y, z} {}
        constexpr Vector3d(const Vector3d<T> &xyz) = default;
        constexpr explicit Vector3d(const T xyz) : _xyz{xyz, xyz, xyz} {}

        using value_type = T;

        /************************
         * assignment operators *
         ************************/

        // copy assignment operators
        constexpr Vector3d &operator=(Vector3d<T> &);
        constexpr Vector3d &operator=(const Vector3d<T> &);

        /**********************
         * indexing operators *
         **********************/

        constexpr T       &operator[](const size_t index);
        constexpr const T &operator[](const size_t index) const;
    };

}   // namespace mstd

#include "vector3dClass.tpp.hpp"

#endif   //  __MSTD__LINALG__VECTOR3D_CLASS_HPP__
