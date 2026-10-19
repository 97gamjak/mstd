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

#ifndef __MSTD__TYPE_TRAITS__QUANTITY_TRAITS_HPP__
#define __MSTD__TYPE_TRAITS__QUANTITY_TRAITS_HPP__

/**
 * @file quantity_traits.hpp
 * @brief Concepts for quantity, unit, dimension, and dimension ratio types.
 *
 * Provides concepts to check if a type conforms to the expected interface
 * of quantity types, unit types, dimension types, and dimension ratio types.
 */

#include <type_traits>
namespace mstd
{
    /**
     * @brief Concept to check if a type is Quantity-like.
     *
     * A type satisfies this concept if it has the nested types `rep`, `unit`,
     * and `reference`, and provides a member function `numerical_value_in` that
     * accepts its `unit` type.
     */
    template <typename T>
    concept QuantityLike = requires(const T &q) {
        typename T::rep;
        T::unit;
        T::reference;
        q.numerical_value_in(T::unit);
    };

    /**
     * @brief Variable template to check if a type is Quantity-like.
     *
     * Evaluates to `true` if the type satisfies the `QuantityLike` concept,
     * otherwise evaluates to `false`.
     */
    template <typename T>
    static constexpr bool is_quantity_like_v = QuantityLike<T>;

    /**
     * @brief Concept to check if a type is a non-arithmetic scaler for a
     * quantity.
     *
     * A type `R` satisfies this concept with respect to a type `T` if `R` is
     * not an arithmetic type and `T` can be multiplied by `R`.
     */
    template <typename T, typename R>
    concept NonArithmeticScaler =
        !std::is_arithmetic_v<R> && requires(const T &t, const R &r) { t * r; };

}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__QUANTITY_TRAITS_HPP__
