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

#ifndef __MSTD__TYPE_TRAITS__MATH__ARITHMETIC_TRAITS_HPP__
#define __MSTD__TYPE_TRAITS__MATH__ARITHMETIC_TRAITS_HPP__

namespace mstd
{
    /**
     * @brief concept for addition
     *
     * @tparam T
     */
    template <typename T>
    concept Addable = requires(T a, T b) {
        { a + b } -> std::convertible_to<T>;
    };

    /**
     * @brief checks if addition is possible
     *
     * @tparam T
     */
    template <typename T>
    static constexpr bool is_addable_v = Addable<T>;

    /**
     * @brief concept for subtraction
     *
     * @tparam T
     */
    template <typename T>
    concept Subtractable = requires(T a, T b) {
        { a - b } -> std::convertible_to<T>;
    };

    /**
     * @brief checks if subtraction is possible
     *
     * @tparam T
     */
    template <typename T>
    static constexpr bool is_subtractable_v = Subtractable<T>;

    /**
     * @brief concept for multiplication
     *
     * @tparam T
     */
    template <typename T>
    concept Multipliable = requires(T a, T b) {
        { a * b } -> std::convertible_to<T>;
    };

    /**
     * @brief checks if multiplication is possible
     *
     * @tparam T
     */
    template <typename T>
    static constexpr bool is_multipliable_v = Multipliable<T>;

    /**
     * @brief concept for division
     *
     * @tparam T
     */
    template <typename T>
    concept Divisible = requires(T a, T b) {
        { a / b } -> std::convertible_to<T>;
    };

    template <typename T>
    static constexpr bool is_divisible_v = Divisible<T>;

}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__MATH__ARITHMETIC_TRAITS_HPP__