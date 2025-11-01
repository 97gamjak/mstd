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

#ifndef __MSTD_TYPE_TRAITS_MATH_TRAITS_HPP__
#define __MSTD_TYPE_TRAITS_MATH_TRAITS_HPP__

#include "mstd/error.hpp"

namespace mstd
{

    /**
     * @brief Concept that checks if a type is a rational number type.
     *
     * @tparam T
     */
    template <typename T>
    concept RationalType = requires {
        T::num;
        T::den;
        T::gcd;
    };

    /**
     * @brief Checks if a type is a rational number type.
     *
     * @tparam T
     */
    template <typename T>
    constexpr bool is_rational_v = RationalType<T>;

    template <typename T>
    concept RationalPowerType = requires {
        typename T::base;
        typename T::exponent;
    };

    /**
     * @brief Checks if a type is a RationalPower type.
     *
     * @tparam T
     */
    template <typename T>
    constexpr bool is_rational_power_v = RationalPowerType<T>;

    template <typename T>
    constexpr bool is_convertible_to_rational_power_v =
        is_rational_power_v<T> || is_rational_v<T>;

    /**
     * @brief Metafunction to determine the resulting type of adding two types.
     *
     * @note Specializations must be provided for supported type combinations.
     *
     * @tparam T1 First type
     * @tparam T2 Second type
     */
    template <typename T1, typename T2>
    struct add_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for add type");
    };

    /**
     * @brief Alias for add_type metafunction.
     *
     * @tparam T1 First type
     * @tparam T2 Second type
     */
    template <typename T1, typename T2>
    using add_type_t = typename add_type<T1, T2>::type;

    template <typename T1, typename T2>
    struct sub_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for sub type");
    };

    template <typename T1, typename T2>
    using sub_type_t = typename sub_type<T1, T2>::type;

    template <typename T1, typename T2>
    struct pow_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for pow type");
    };

    template <typename T1, typename T2>
    using pow_type_t = typename pow_type<T1, T2>::type;

    template <typename T1, typename T2>
    struct mul_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for mul type");
    };

    template <typename T1, typename T2>
    using mul_type_t = typename mul_type<T1, T2>::type;

    template <typename T1, typename T2>
    struct div_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for div type");
    };

    template <typename T1, typename T2>
    using div_type_t = typename div_type<T1, T2>::type;

}   // namespace mstd

#endif   // __MSTD_TYPE_TRAITS_MATH_TRAITS_HPP__