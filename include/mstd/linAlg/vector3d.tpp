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

#ifndef __MSTD__LINALG__VECTOR3D_TPP__
#define __MSTD__LINALG__VECTOR3D_TPP__

#include <mp-units/math.h>   // for mp_units::sqrt

#include <cmath>   // for std::sqrt
#include <concepts>
#include <utility>

#include "vector3d.hpp"

namespace mstd
{
    /************************
     * comparison operators *
     ************************/

    template <typename U, typename V>
    requires std::equality_comparable_with<U, V>
    constexpr bool operator==(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) noexcept
    {
        return lhs[0] == rhs[0] && lhs[1] == rhs[1] && lhs[2] == rhs[2];
    }

    template <typename U, typename V>
    requires std::equality_comparable_with<U, V>
    constexpr bool operator!=(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) noexcept
    {
        return !(lhs == rhs);
    }

    /*********************
     * binary + operator *
     *********************/

    template <typename U, typename V>
    requires requires(const U &u, const V &v) { u + v; }
    [[nodiscard]] constexpr auto operator+(
        const Vector3d<U> &lhs,
        const Vector3d<V> &rhs
    ) -> Vector3d<decltype(std::declval<U>() + std::declval<V>())>
    {
        using ResultType = decltype(std::declval<U>() + std::declval<V>());

        return Vector3d<ResultType>(
            lhs[0] + rhs[0],
            lhs[1] + rhs[1],
            lhs[2] + rhs[2]
        );
    }

    template <typename U, Vector3dConcept V>
    requires requires(const U &scalar, const V &vector) {
        scalar + vector[0];
    } && (!Vector3dConcept<U>)
    constexpr auto operator+(const U &scalar, const V &vector)
        -> Vector3d<decltype(scalar + vector[0])>
    {
        using ResultType = decltype(scalar + vector[0]);

        return Vector3d<ResultType>(
            scalar + vector[0],
            scalar + vector[1],
            scalar + vector[2]
        );
    }

    template <Vector3dConcept U, typename V>
    requires requires(const U &vector, const V &scalar) {
        vector[0] + scalar;
    } && (!Vector3dConcept<V>)
    constexpr auto operator+(const U &vector, const V &scalar)
        -> Vector3d<decltype(vector[0] + scalar)>
    {
        using ResultType = decltype(vector[0] + scalar);

        return Vector3d<ResultType>(
            vector[0] + scalar,
            vector[1] + scalar,
            vector[2] + scalar
        );
    }

    /*********************
     * binary - operator *
     *********************/

    template <Vector3dConcept U, Vector3dConcept V>
    requires requires(const U &lhs, const V &rhs) { lhs[0] - rhs[0]; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    constexpr auto operator-(const U &lhs, const V &rhs)
        -> Vector3d<decltype(lhs[0] - rhs[0])>
    {
        using ResultType = decltype(lhs[0] - rhs[0]);

        return Vector3d<ResultType>(
            lhs[0] - rhs[0],
            lhs[1] - rhs[1],
            lhs[2] - rhs[2]
        );
    }

    template <typename U, Vector3dConcept V>
    requires requires(const U &scalar, const V &vector) {
        scalar - vector[0];
    } && (!Vector3dConcept<U>)
    constexpr auto operator-(const U &scalar, const V &vector)
        -> Vector3d<decltype(scalar - vector[0])>
    {
        using ResultType = decltype(scalar - vector[0]);

        return Vector3d<ResultType>(
            scalar - vector[0],
            scalar - vector[1],
            scalar - vector[2]
        );
    }

    template <Vector3dConcept U, typename V>
    requires requires(const U &vector, const V &scalar) {
        vector[0] - scalar;
    } && (!Vector3dConcept<V>)
    constexpr auto operator-(const U &vector, const V &scalar)
        -> Vector3d<decltype(vector[0] - scalar)>
    {
        using ResultType = decltype(vector[0] - scalar);

        return Vector3d<ResultType>(
            vector[0] - scalar,
            vector[1] - scalar,
            vector[2] - scalar
        );
    }

    /*********************
     * binary * operator *
     *********************/

    template <Vector3dConcept U, Vector3dConcept V>
    requires requires(const U &lhs, const V &rhs) { lhs[0] * rhs[0]; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    constexpr auto operator*(const U &lhs, const V &rhs)
        -> Vector3d<decltype(lhs[0] * rhs[0])>
    {
        using ResultType = decltype(lhs[0] * rhs[0]);

        return Vector3d<ResultType>(
            lhs[0] * rhs[0],
            lhs[1] * rhs[1],
            lhs[2] * rhs[2]
        );
    }

    template <typename U, Vector3dConcept V>
    requires requires(const U &scalar, const V &vector) {
        scalar * vector[0];
    } && (!Vector3dConcept<U>)
    constexpr auto operator*(const U &scalar, const V &vector)
        -> Vector3d<decltype(scalar * vector[0])>
    {
        using ResultType = decltype(scalar * vector[0]);

        return Vector3d<ResultType>(
            scalar * vector[0],
            scalar * vector[1],
            scalar * vector[2]
        );
    }

    template <Vector3dConcept U, typename V>
    requires requires(const U &vector, const V &scalar) {
        vector[0] * scalar;
    } && (!Vector3dConcept<V>)
    constexpr auto operator*(const U &vector, const V &scalar)
        -> Vector3d<decltype(vector[0] * scalar)>
    {
        using ResultType = decltype(vector[0] * scalar);

        return Vector3d<ResultType>(
            vector[0] * scalar,
            vector[1] * scalar,
            vector[2] * scalar
        );
    }

    /*********************
     * binary / operator *
     *********************/

    template <Vector3dConcept U, Vector3dConcept V>
    requires requires(const U &lhs, const V &rhs) { lhs[0] / rhs[0]; } &&
             (Vector3dDepthDifference_v<U, V> == 0)
    constexpr auto operator/(const U &lhs, const V &rhs)
        -> Vector3d<decltype(lhs[0] / rhs[0])>
    {
        using ResultType = decltype(lhs[0] / rhs[0]);

        return Vector3d<ResultType>(
            lhs[0] / rhs[0],
            lhs[1] / rhs[1],
            lhs[2] / rhs[2]
        );
    }

    template <typename U, Vector3dConcept V>
    requires requires(const U &scalar, const V &vector) {
        scalar / vector[0];
    } && (!Vector3dConcept<U>)
    constexpr auto operator/(const U &scalar, const V &vector)
        -> Vector3d<decltype(scalar / vector[0])>
    {
        using ResultType = decltype(scalar / vector[0]);

        return Vector3d<ResultType>(
            scalar / vector[0],
            scalar / vector[1],
            scalar / vector[2]
        );
    }

    template <Vector3dConcept U, typename V>
    requires requires(const U &vector, const V &scalar) {
        vector[0] / scalar;
    } && (!Vector3dConcept<V>)
    constexpr auto operator/(const U &vector, const V &scalar)
        -> Vector3d<decltype(vector[0] / scalar)>
    {
        using ResultType = decltype(vector[0] / scalar);

        return Vector3d<ResultType>(
            vector[0] / scalar,
            vector[1] / scalar,
            vector[2] / scalar
        );
    }

    /******************
     * norm functions *
     ******************/

    template <Vector3dConcept U>
    requires requires(const U &vec) { std::sqrt(normSquared(vec)); }
    [[nodiscard]] auto norm(const U &vec)
        -> decltype(std::sqrt(normSquared(vec)))
    {
        return std::sqrt(normSquared(vec));
    }

    template <Vector3dConcept U>
    requires requires(const U &vec) { mp_units::sqrt(normSquared(vec)); }
    [[nodiscard]] auto norm(const U &vec)
        -> decltype(mp_units::sqrt(normSquared(vec)))
    {
        return mp_units::sqrt(normSquared(vec));
    }

    template <Vector3dConcept U>
    requires requires(const U &vec) {
        vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    }
    [[nodiscard]] constexpr auto normSquared(const U &vec)
        -> decltype(vec[0] * vec[0])
    {
        return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
    }

    /**************
     * ostream << *
     **************/

    template <Vector3dConcept U>
    requires requires(std::ostream &os, const U &vector) {
        { os << vector[0] } -> std::same_as<std::ostream &>;
    }
    std::ostream &operator<<(std::ostream &os, const U &vector)
    {
        return os << vector[0] << " " << vector[1] << " " << vector[2];
    }

}   // namespace mstd

#endif   //  __MSTD__LINALG__VECTOR3D_TPP__
