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

#include <utility>   // for std::declval

#include "vector3dClass.hpp"

namespace mstd
{
    /***********************
     *                     *
     * unary +/- operators *
     *                     *
     ***********************/

    template <typename T>
    constexpr Vector3d<T> Vector3d<T>::operator+() const
    {
        return *this;
    }

    template <typename T>
    constexpr Vector3d<T> Vector3d<T>::operator-() const
    {
        return Vector3d<T>{-_xyz[0], -_xyz[1], -_xyz[2]};
    }

    /*******************
     * unit conversion *
     *******************/

    template <typename T>
    template <typename U>
    requires requires(const T &v, const U &unit) { v.in(unit); }
    constexpr auto Vector3d<T>::in(const U &unit) const
    {
        using ResultType = decltype(std::declval<const T &>().in(unit));

        return Vector3d<ResultType>{
            _xyz[0].in(unit),
            _xyz[1].in(unit),
            _xyz[2].in(unit),
        };
    }

    template <typename T>
    template <typename U>
    requires requires(const T &v, const U &unit) { v.force_in(unit); }
    constexpr auto Vector3d<T>::force_in(const U &unit) const
    {
        using ResultType = decltype(std::declval<const T &>().force_in(unit));

        return Vector3d<ResultType>{
            _xyz[0].force_in(unit),
            _xyz[1].force_in(unit),
            _xyz[2].force_in(unit),
        };
    }

    template <typename T>
    template <typename U>
    requires requires(const T &v, const U &unit) { v.numerical_value_in(unit); }
    constexpr auto Vector3d<T>::numerical_value_in(const U &unit) const
    {
        using ResultType =
            decltype(std::declval<const T &>().numerical_value_in(unit));

        return Vector3d<ResultType>{
            _xyz[0].numerical_value_in(unit),
            _xyz[1].numerical_value_in(unit),
            _xyz[2].numerical_value_in(unit),
        };
    }

    template <typename T>
    template <typename U>
    requires requires(const T &v, const U &unit) {
        v.force_numerical_value_in(unit);
    }
    constexpr auto Vector3d<T>::force_numerical_value_in(const U &unit) const
    {
        using ResultType =
            decltype(std::declval<const T &>().force_numerical_value_in(unit));

        return Vector3d<ResultType>{
            _xyz[0].force_numerical_value_in(unit),
            _xyz[1].force_numerical_value_in(unit),
            _xyz[2].force_numerical_value_in(unit),
        };
    }

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
