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

#ifndef __MSTD_UNITS_QUANTITY_HPP__
#define __MSTD_UNITS_QUANTITY_HPP__

#include "mstd/error.hpp"

MSTD_WARN_BUGGY_LIBRARY("mstd/quantity.hpp")

#include <type_traits>

#include "mstd/type_traits/quantity_traits.hpp"
#include "unit_operations.hpp"

namespace mstd
{

    /**
     * @brief A Quantity represents a physical Quantity with a unit and a value.
     *
     * @tparam U The unit type.
     * @tparam Rep The representation type (default: double).
     */
    template <UnitType U, class Rep = double>
    class Quantity
    {
       public:
        using unit = U;
        using rep  = Rep;

        struct from_base_t
        {
            explicit from_base_t() = default;
        };
        static constexpr from_base_t from_base_tag{};

       private:
        Rep _baseValue;

       public:
        /**
         * @brief Construct a Quantity from a value.
         *
         * @param v The value to convert.
         */
        constexpr explicit Quantity(Rep v)
            : _baseValue(v * static_cast<Rep>(scale_v<unit>))
        {
        }

        /**
         * @brief Construct a Quantity from a base value (SI unit).
         *
         * @param base The base value in SI units.
         */
        constexpr Quantity(from_base_t, Rep base)   // base (SI) ctor
            : _baseValue(base)
        {
        }

        /**
         * @brief Get the value of the Quantity in its unit.
         *
         * @return The value in the specified unit.
         */
        constexpr Rep value() const
        {
            return _baseValue / static_cast<Rep>(scale_v<unit>);
        }

        /**
         * @brief Get the base value of the Quantity in SI units.
         *
         * @return The base value in SI units.
         */
        constexpr Rep baseValue() const { return _baseValue; }
    };

    /**
     * @brief Construct a Quantity from a base value (SI unit).
     *
     * @tparam U The unit type.
     * @tparam Rep The representation type (default: double).
     * @param v The base value in SI units.
     * @return constexpr auto
     */
    template <class U, class Rep = double>
    constexpr auto qty(Rep v)
    {
        return Quantity<U, Rep>(v);
    }

    /**
     * @brief Convert a Quantity to another unit.
     *
     * @tparam UTo The target unit type.
     * @tparam UFrom The source unit type.
     * @tparam R The representation type (default: double).
     * @param q The Quantity to convert.
     * @return constexpr Quantity<UTo, R>
     */
    template <class UTo, class UFrom, class R>
    constexpr Quantity<UTo, R> to(Quantity<UFrom, R> q)
    requires same_dimension_v<UFrom, UTo>
    {
        return Quantity<UTo, R>(
            typename Quantity<UTo, R>::from_base_t{},
            q.baseValue()
        );
    }

    /**
     * @brief Convert a Quantity to a scalar value by dividing by another
     * Quantity of the same dimension.
     *
     * @tparam Rep The representation type (default: double).
     * @tparam U1 The unit type of the numerator.
     * @tparam U2 The unit type of the denominator.
     * @param a The numerator Quantity.
     * @param b The denominator Quantity.
     * @return Rep The resulting scalar value.
     */
    template <class Rep, class U1, class U2>
    requires same_dimension_v<U1, U2>
    constexpr Rep quantity_cast(Quantity<U1, Rep> a, Quantity<U2, Rep> b)
    {
        return to<U1>(b).value() ? a.value() / to<U1>(b).value() : Rep{};
    }

    /**
     * @brief Equality operator
     *
     * @tparam U1 The unit type of the first Quantity.
     * @tparam U2 The unit type of the second Quantity.
     * @tparam R1 The representation type of the first Quantity.
     * @tparam R2 The representation type of the second Quantity.
     * @param a The first Quantity.
     * @param b The second Quantity.
     * @return constexpr bool True if the quantities are equal, false otherwise.
     */
    template <class U1, class U2, class R1, class R2>
    constexpr bool operator==(Quantity<U1, R1> a, Quantity<U2, R2> b)
    {
        auto ret  = same_dimension_v<U1, U2>;
        ret      &= a.baseValue() == b.baseValue();

        return ret;
    }

    /**
     * @brief Addition operator for quantities with the same unit.
     *
     * @tparam U The unit type.
     * @tparam R1 The representation type of the first Quantity.
     * @tparam R2 The representation type of the second Quantity.
     * @param a The first Quantity.
     * @param b The second Quantity.
     * @return constexpr auto The resulting Quantity after addition.
     */
    template <class U, class R1, class R2>
    constexpr auto operator+(const Quantity<U, R1>& a, const Quantity<U, R2>& b)
    {
        using R   = std::common_type_t<R1, R2>;
        const R v = static_cast<R>(a.value()) + static_cast<R>(b.value());

        return Quantity<U, R>(v);
    }

    /**
     * @brief Subtraction operator for quantities with the same unit.
     *
     * @tparam U The unit type.
     * @tparam R1 The representation type of the first Quantity.
     * @tparam R2 The representation type of the second Quantity.
     * @param a The first Quantity.
     * @param b The second Quantity.
     * @return constexpr auto The resulting Quantity after subtraction.
     */
    template <class U, class R1, class R2>
    constexpr auto operator-(const Quantity<U, R1>& a, const Quantity<U, R2>& b)
    {
        using R   = std::common_type_t<R1, R2>;
        const R v = static_cast<R>(a.value()) - static_cast<R>(b.value());

        return Quantity<U, R>(v);
    }

    /**
     * @brief Addition operator for quantities with compatible units.
     *
     * @tparam Unit1 The unit type of the first Quantity.
     * @tparam Unit2 The unit type of the second Quantity.
     * @tparam R1 The representation type of the first Quantity.
     * @tparam R2 The representation type of the second Quantity.
     * @param a The first Quantity.
     * @param b The second Quantity.
     * @return requires constexpr auto The resulting Quantity after addition.
     * @return requires constexpr auto The resulting Quantity after subtraction.
     * @note The resulting Quantity will have the common unit of the two input
     * quantities.
     */
    template <class Unit1, class Unit2, class R1, class R2>
    requires same_dimension_v<Unit1, Unit2>
    constexpr auto operator+(
        const Quantity<Unit1, R1>& a,
        const Quantity<Unit2, R2>& b
    )
    {
        using unit      = common_unit_t<Unit1, Unit2>;
        const auto aNew = to<unit>(a);
        const auto bNew = to<unit>(b);

        return aNew + bNew;
    }

    /**
     * @brief Subtraction operator for quantities with compatible units.
     *
     * @tparam Unit1 The unit type of the first Quantity.
     * @tparam Unit2 The unit type of the second Quantity.
     * @tparam R1 The representation type of the first Quantity.
     * @tparam R2 The representation type of the second Quantity.
     * @param a The first Quantity.
     * @param b The second Quantity.
     * @return requires constexpr auto The resulting Quantity after subtraction.
     * @note The resulting Quantity will have the common unit of the two input
     * quantities.
     */
    template <class Unit1, class Unit2, class R1, class R2>
    requires same_dimension_v<Unit1, Unit2>
    constexpr auto operator-(
        const Quantity<Unit1, R1>& a,
        const Quantity<Unit2, R2>& b
    )
    {
        using unit         = common_unit_t<Unit1, Unit2>;
        using R            = std::common_type_t<R1, R2>;
        const auto aCommon = to<unit>(a);
        const auto bCommon = to<unit>(b);
        const R    v =
            static_cast<R>(aCommon.value()) - static_cast<R>(bCommon.value());

        return Quantity<unit, R>(v);
    }

    /**
     * @brief Multiplication operator for quantities
     *
     * @tparam Unit1 The unit type of the first Quantity.
     * @tparam Unit2 The unit type of the second Quantity.
     * @tparam R1 The representation type of the first Quantity.
     * @tparam R2 The representation type of the second Quantity.
     * @param a The first Quantity.
     * @param b The second Quantity.
     * @return requires constexpr auto The resulting Quantity after
     * multiplication.
     * @note The resulting Quantity will have the product unit of the two input
     * quantities.
     */
    template <class U1, class R1, class U2, class R2>
    constexpr auto operator*(Quantity<U1, R1> a, Quantity<U2, R2> b)
    {
        using U   = unit_mul_t<U1, U2>;
        using R   = std::common_type_t<R1, R2>;
        const R v = static_cast<R>(a.value()) * static_cast<R>(b.value());

        return Quantity<U, R>(v);
    }

    /**
     * @brief Division operator for quantities
     *
     * @tparam Unit1 The unit type of the numerator Quantity.
     * @tparam Unit2 The unit type of the denominator Quantity.
     * @tparam R1 The representation type of the numerator Quantity.
     * @tparam R2 The representation type of the denominator Quantity.
     * @param a The numerator Quantity.
     * @param b The denominator Quantity.
     * @return requires constexpr auto The resulting Quantity after division.
     * @note The resulting Quantity will have the quotient unit of the two input
     * quantities.
     */
    template <class U1, class R1, class U2, class R2>
    constexpr auto operator/(Quantity<U1, R1> a, Quantity<U2, R2> b)
    {
        using U   = unit_div_t<U1, U2>;
        using R   = std::common_type_t<R1, R2>;
        const R v = static_cast<R>(a.value()) / static_cast<R>(b.value());

        return Quantity<U, R>(v);
    }

    /**
     * @brief Multiplication operator for Quantity and scalar
     *
     * @tparam U The unit type of the Quantity.
     * @tparam R1 The representation type of the Quantity.
     * @tparam S The scalar type.
     * @param q The Quantity.
     * @param s The scalar value.
     * @return constexpr auto The resulting Quantity after multiplication.
     */
    template <class U, class R1, class S>
    constexpr auto operator*(Quantity<U, R1> q, S s)
    requires std::is_arithmetic_v<S>
    {
        using R = std::common_type_t<R1, S>;
        return Quantity<U, R>(static_cast<R>(q.value()) * static_cast<R>(s));
    }

    /**
     * @brief Multiplication operator for scalar and Quantity
     *
     * @tparam U The unit type of the Quantity.
     * @tparam R1 The representation type of the Quantity.
     * @tparam S The scalar type.
     * @param s The scalar value.
     * @param q The Quantity.
     * @return constexpr auto The resulting Quantity after multiplication.
     */
    template <class U, class R1, class S>
    constexpr auto operator*(S s, Quantity<U, R1> q)
    requires std::is_arithmetic_v<S>
    {
        return q * s;
    }

    /**
     * @brief Division operator for Quantity and scalar
     *
     * @tparam U The unit type of the Quantity.
     * @tparam R1 The representation type of the Quantity.
     * @tparam S The scalar type.
     * @param q The Quantity.
     * @param s The scalar value.
     * @return constexpr auto The resulting Quantity after division.
     */
    template <class U, class R1, class S>
    constexpr auto operator/(Quantity<U, R1> q, S s)
    requires std::is_arithmetic_v<S>
    {
        using R = std::common_type_t<R1, S>;
        return Quantity<U, R>(static_cast<R>(q.value()) / static_cast<R>(s));
    }

}   // namespace mstd

#endif   // __MSTD_UNITS_QUANTITY_HPP__
