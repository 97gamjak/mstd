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

#ifndef __MSTD__LINALG__VECTOR3D_HPP__
#define __MSTD__LINALG__VECTOR3D_HPP__

#include "concepts/vector3dConcepts.hpp"
#include "vector3dClass.hpp"

namespace mstd
{
    /************************
     * comparison operators *
     ************************/

    template <typename U, typename V>
    requires requires(const Vector3d<U> &lhs, const Vector3d<V> &rhs) {
        { lhs[0] == rhs[0] } -> std::convertible_to<bool>;
    }
    constexpr bool operator==(const Vector3d<U> &lhs, const Vector3d<V> &rhs);

    /*********************
     * binary * operator *
     *********************/

    template <Vector3dConcept U, Vector3dConcept V>
    requires requires(const U &a, const V &b) { a[0] * b[0]; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    constexpr auto operator*(const U &lhs, const V &rhs)
        -> Vector3d<decltype(lhs[0] * rhs[0])>;

    /*********************
     * binary / operator *
     *********************/

    template <Vector3dConcept U, Vector3dConcept V>
    requires requires(const U &a, const V &b) { a[0] / b[0]; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    constexpr auto operator/(const U &lhs, const V &rhs)
        -> Vector3d<decltype(lhs[0] / rhs[0])>;

    /**************
     * ostream << *
     **************/

    template <Vector3dConcept U>
    requires requires(std::ostream &os, const U &a) {
        { os << a[0] } -> std::same_as<std::ostream &>;
    }
    std::ostream &operator<<(std::ostream &os, U const &v);

}   // namespace mstd

#include "vector3d.tpp"

#endif   //  __MSTD__LINALG__VECTOR3D_HPP__
