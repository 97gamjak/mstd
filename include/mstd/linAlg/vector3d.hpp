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

#include <mp-units/math.h>   // for mp_units::sqrt

#include <cmath>   // for std:sqrt
#include <concepts>
#include <ostream>   // for std::ostream
#include <utility>

#include "concepts/vector3dConcepts.hpp"
#include "vector3dClass.hpp"   // IWYU pragma: export

namespace mstd
{
    /************************
     * comparison operators *
     ************************/

    template <typename U, typename V>
    requires std::equality_comparable_with<U, V>
    [[nodiscard]] constexpr bool operator==(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) noexcept;

    template <typename U, typename V>
    requires std::equality_comparable_with<U, V>
    [[nodiscard]] constexpr bool operator!=(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) noexcept;

    /*********************
     * binary + operator *
     *********************/

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u + v; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    [[nodiscard]] constexpr auto operator+(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) -> Vector3d<decltype(std::declval<U>() + std::declval<V>())>;

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u + v; }
    [[nodiscard]] constexpr auto operator+(
        const U           &scalar,
        const Vector3d<V> &vector
    ) -> Vector3d<decltype(std::declval<U>() + std::declval<V>())>;

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u + v; }
    [[nodiscard]] constexpr auto operator+(
        const Vector3d<U> &vector,
        const V           &scalar
    ) -> Vector3d<decltype(std::declval<U>() + std::declval<V>())>;

    /*********************
     * binary - operator *
     *********************/

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u - v; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    [[nodiscard]] constexpr auto operator-(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) -> Vector3d<decltype(std::declval<U>() - std::declval<V>())>;

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u - v; }
    [[nodiscard]] constexpr auto operator-(
        const Vector3d<U> &vector,
        const V           &scalar
    ) -> Vector3d<decltype(std::declval<U>() - std::declval<V>())>;

    /*********************
     * binary * operator *
     *********************/

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u * v; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    [[nodiscard]] constexpr auto operator*(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) -> Vector3d<decltype(std::declval<U>() * std::declval<V>())>;

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u * v; }
    [[nodiscard]] constexpr auto operator*(
        const U           &scalar,
        const Vector3d<V> &vector
    ) -> Vector3d<decltype(std::declval<U>() * std::declval<V>())>;

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u * v; }
    [[nodiscard]] constexpr auto operator*(
        const Vector3d<U> &vector,
        const V           &scalar
    ) -> Vector3d<decltype(std::declval<U>() * std::declval<V>())>;

    /*********************
     * binary / operator *
     *********************/

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u / v; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    [[nodiscard]] constexpr auto operator/(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) -> Vector3d<decltype(std::declval<U>() / std::declval<V>())>;

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u / v; }
    [[nodiscard]] constexpr auto operator/(
        const Vector3d<U> &vector,
        const V           &scalar
    ) -> Vector3d<decltype(std::declval<U>() / std::declval<V>())>;

    /******************
     * norm functions *
     ******************/

    template <Vector3dConcept U>
    requires requires(const U &vec) { std::sqrt(normSquared(vec)); }
    [[nodiscard]] auto norm(const U &vec)
        -> decltype(std::sqrt(normSquared(vec)));

    template <Vector3dConcept U>
    requires requires(const U &vec) { mp_units::sqrt(normSquared(vec)); }
    [[nodiscard]] auto norm(const U &vec)
        -> decltype(mp_units::sqrt(normSquared(vec)));

    template <Vector3dConcept U>
    requires requires(const U &vec) {
        vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    }
    [[nodiscard]] constexpr auto normSquared(const U &vec)
        -> decltype(vec[0] * vec[0]);

    /**************
     * ostream << *
     **************/

    template <Vector3dConcept U>
    requires requires(std::ostream &os, const U &vector) {
        { os << vector[0] } -> std::same_as<std::ostream &>;
    }
    std::ostream &operator<<(std::ostream &os, const U &vector);

}   // namespace mstd

#include "vector3d.tpp"

#endif   //  __MSTD__LINALG__VECTOR3D_HPP__
