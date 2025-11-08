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

/**
 * @file quantity_traits.hpp
 * @brief Concepts for quantity, unit, dimension, and dimension ratio types.
 *
 * Provides concepts to check if a type conforms to the expected interface
 * of quantity types, unit types, dimension types, and dimension ratio types.
 */

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
        RationalPackType<typename T::si> && RationalPackType<typename T::ex>;

    template <typename T>
    constexpr bool is_dim_v = DimType<T>;

    /**
     * @brief Concept for dimension ratio types.
     *
     * @details A dimension ratio type must provide two nested types `si` and
     * `ex` which are both ratio packs.
     */
    template <typename T>
    concept DimRatioType = RationalPowerPackType<typename T::si> &&
                           RationalPowerPackType<typename T::ex>;

    template <typename T>
    constexpr bool is_dim_ratio_v = DimRatioType<T>;

    template <typename T>
    concept UnitType = DimRatioType<typename T::dimRatio> &&
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

    /**
     * @brief Checks if two units have the same dimension
     *
     * @tparam Unit1 First unit
     * @tparam Unit2 Second unit
     * @return true if both units have the same dimension, false otherwise
     */
    template <UnitType U1, UnitType U2>
    inline constexpr bool same_dimension_v =
        std::is_same_v<typename U1::dim, typename U2::dim>;

    // default case false see below specializations
    template <typename T, typename U>
    inline constexpr bool has_dim_v = false;

    /**
     * @brief Check if a unit has a specific dimension.
     *
     * @tparam Unit The unit type.
     * @tparam Dim The dimension type to check against.
     * @return true if the unit has the specified dimension, false otherwise.
     */
    template <UnitType Unit, DimType Dim>
    inline constexpr bool has_dim_v<Unit, Dim> =
        std::is_same_v<typename Unit::dim, Dim>;

    /**
     * @brief Check if a quantity has a specific dimension.
     *
     * @tparam Q The quantity type.
     * @tparam U The unit type to check against.
     * @return true if the quantity has the specified dimension, false
     * otherwise.
     */
    template <QuantityType Q, DimType Dim>
    inline constexpr bool has_dim_v<Q, Dim> =
        std::is_same_v<typename Q::unit::dim, Dim>;

}   // namespace mstd

#endif   // __MSTD_TYPE_TRAITS_QUANTITY_TRAITS_HPP__