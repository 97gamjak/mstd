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

#ifndef __MSTD_TYPE_TRAITS_QUANTITY_TRAITS_HPP__
#define __MSTD_TYPE_TRAITS_QUANTITY_TRAITS_HPP__

#include "pack_traits.hpp"
#include "ratio_traits.hpp"

namespace mstd
{
    /**
     * @brief Concept for dimension types.
     *
     * @details A dimension type must provide two nested types `si` and `ex`
     * which are both integer packs.
     */
    template <typename T>
    concept DimType =
        IntegerPackType<typename T::si> && IntegerPackType<typename T::ex>;

    template <typename T>
    constexpr bool is_dim_v = DimType<T>;

    /**
     * @brief Concept for dimension ratio types.
     *
     * @details A dimension ratio type must provide two nested types `si` and
     * `ex` which are both ratio packs.
     */
    template <typename T>
    concept DimRatioType =
        RatioPackType<typename T::si> && RatioPackType<typename T::ex>;

    template <typename T>
    constexpr bool is_dim_ratio_v = DimRatioType<T>;

    template <typename T>
    concept UnitType =
        DimType<typename T::dim> && DimRatioType<typename T::ratio> &&
        RatioType<typename T::global> && requires {
            { T::factor_v } -> std::convertible_to<long double>;
        } && (T::factor_v != 0.0L);

    template <typename T>
    constexpr bool is_unit_v = UnitType<T>;

    /**
     * @brief Concept for quantity types.
     *
     * @details A quantity type must provide a nested type `unit` which is a
     * unit type and a nested type `rep` which is the representation type.
     */
    template <typename T>
    concept QuantityType =
        UnitType<typename T::unit> && requires { typename T::rep; };

    template <typename T>
    constexpr bool is_quantity_v = QuantityType<T>;

}   // namespace mstd

#endif   // __MSTD_TYPE_TRAITS_QUANTITY_TRAITS_HPP__