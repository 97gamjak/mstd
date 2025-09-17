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

#ifndef __MSTD_UNIT_COMMON_HPP__
#define __MSTD_UNIT_COMMON_HPP__

#include "unit_operations.hpp"

namespace mstd::units::details
{
    /**
     * @brief Get the common unit type for two compatible units.
     *
     * @tparam U1 The first unit type.
     * @tparam U2 The second unit type.
     * @tparam Enable SFINAE helper.
     */
    template <class U1, class U2, class Enable = void>
    struct common_unit_impl;

    /**
     * @brief Get the common unit type for two compatible units.
     *
     * @details The common unit is determined by the following rules:
     * 1. if only one has factor==1, pick that one
     * 2. if both factors != 1, pick LHS
     * 3. both factors == 1 → pick smallest ratio (finer); tie → LHS
     *
     * @tparam Unit1 The first unit type.
     * @tparam Unit2 The second unit type.
     */
    template <class Unit1, class Unit2>
    struct common_unit_impl<
        Unit1,
        Unit2,
        std::enable_if_t<compatible_units_v<Unit1, Unit2>>>
    {
        static constexpr long double f1   = factor_v<Unit1>;
        static constexpr long double f2   = factor_v<Unit2>;
        static constexpr bool        one1 = (f1 == 1.0L);
        static constexpr bool        one2 = (f2 == 1.0L);

        static constexpr bool only1 = (one1 && !one2);
        static constexpr bool only2 = (!one1 && one2);
        static constexpr bool both  = (one1 && one2);
        static constexpr bool none  = (!one1 && !one2);

        // rule 1: if only one has factor==1, pick that one
        using rule1 = std::
            conditional_t<only1, Unit1, std::conditional_t<only2, Unit2, void>>;

        // rule 2: if both factors != 1, pick LHS
        using rule2 = std::conditional_t<none, Unit1, void>;

        // rule 3: both factors == 1 → pick smallest ratio (finer); tie → LHS
        static constexpr long double r1 = ratio_v<Unit1>;
        static constexpr long double r2 = ratio_v<Unit2>;

        using rule3 = std::conditional_t<
            both,
            std::conditional_t<
                (r1 < r2),
                Unit1,
                std::conditional_t<(r2 < r1), Unit2, Unit1>>,   // tie → LHS
            void>;

        // select type based on rules
        using type = std::conditional_t<
            !std::is_void_v<rule1>,
            rule1,
            std::conditional_t<!std::is_void_v<rule2>, rule2, rule3>>;
    };

    /**
     * @brief Get the common unit type for two compatible units.
     *
     * @tparam U1 The first unit type.
     * @tparam U2 The second unit type.
     */
    template <class U1, class U2>
    using common_unit_t = typename common_unit_impl<U1, U2>::type;

}   // namespace mstd::units::details

#endif   // __MSTD_UNIT_COMMON_HPP__