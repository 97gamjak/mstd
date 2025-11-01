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

#ifndef __MSTD__TYPE_TRAITS__MATH__TYPE_OPERATIONS_HPP__
#define __MSTD__TYPE_TRAITS__MATH__TYPE_OPERATIONS_HPP__

#include "mstd/error.hpp"

namespace mstd
{

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

    /**
     * @brief Metafunction to determine the resulting type of subtracting two
     * types.
     *
     * @note Specializations must be provided for supported type combinations.
     *
     * @tparam T1 First type
     * @tparam T2 Second type
     */
    template <typename T1, typename T2>
    struct sub_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for sub type");
    };

    /**
     * @brief Alias for sub_type metafunction.
     *
     * @tparam T1 First type
     * @tparam T2 Second type
     */
    template <typename T1, typename T2>
    using sub_type_t = typename sub_type<T1, T2>::type;

    /**
     * @brief Metafunction to determine the resulting type of raising T1 to
     * the power of T2.
     *
     * @note Specializations must be provided for supported type combinations.
     *
     * @tparam T1 Base type
     * @tparam T2 Exponent type
     */
    template <typename T1, typename T2>
    struct pow_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for pow type");
    };

    /**
     * @brief Alias for pow_type metafunction.
     *
     * @tparam T1 Base type
     * @tparam T2 Exponent type
     */
    template <typename T1, typename T2>
    using pow_type_t = typename pow_type<T1, T2>::type;

    /**
     * @brief Metafunction to determine the resulting type of multiplying two
     * types.
     *
     * @note Specializations must be provided for supported type combinations.
     *
     * @tparam T1 First type
     * @tparam T2 Second type
     */
    template <typename T1, typename T2>
    struct mul_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for mul type");
    };

    /**
     * @brief Alias for mul_type metafunction.
     *
     * @tparam T1 First type
     * @tparam T2 Second type
     */
    template <typename T1, typename T2>
    using mul_type_t = typename mul_type<T1, T2>::type;

    /**
     * @brief Metafunction to determine the resulting type of dividing two
     * types.
     *
     * @note Specializations must be provided for supported type combinations.
     *
     * @tparam T1 Numerator type
     * @tparam T2 Denominator type
     */
    template <typename T1, typename T2>
    struct div_type
    {
        MSTD_COMPILE_FAIL("Unsupported type specialization for div type");
    };

    /**
     * @brief Alias for div_type metafunction.
     *
     * @tparam T1 Numerator type
     * @tparam T2 Denominator type
     */
    template <typename T1, typename T2>
    using div_type_t = typename div_type<T1, T2>::type;

}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__MATH__TYPE_OPERATIONS_HPP__