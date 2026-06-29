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

#ifndef __MSTD__LINALG__VECTOR3DCLASS_HPP__
#define __MSTD__LINALG__VECTOR3DCLASS_HPP__

#include <array>

namespace mstd
{
    template <typename T>
    class Vector3d
    {
       private:
        std::array<T, 3> _xyz;

       public:
        // clang-format off
        [[nodiscard]] constexpr  Vector3d()                               noexcept = default;
                      constexpr ~Vector3d()                               noexcept = default;
        [[nodiscard]] constexpr  Vector3d(const Vector3d<T> &)            noexcept = default;
                      constexpr  Vector3d &operator=(const Vector3d<T> &) noexcept = default;
        [[nodiscard]] constexpr  Vector3d(Vector3d<T> &&)                 noexcept = default;
                      constexpr  Vector3d &operator=(Vector3d<T> &&)      noexcept = default;
                      
        [[nodiscard]] constexpr explicit Vector3d(const T &xyz)                       noexcept : _xyz{xyz, xyz, xyz} {}
        [[nodiscard]] constexpr          Vector3d(const T &x, const T &y, const T &z) noexcept : _xyz{x, y, z} {}
        // clang-format on

        using value_type = T;

        /**********************
         * indexing operators *
         **********************/

        constexpr T &operator[](const std::size_t index) noexcept;
        [[nodiscard]] constexpr const T &operator[](const std::size_t index
        ) const noexcept;
    };

}   // namespace mstd

#include "vector3dClass.tpp"

#endif   //  __MSTD__LINALG__VECTOR3DCLASS_HPP__
