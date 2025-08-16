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

#include <iomanip>
#include <iostream>
#include <string>

#include "dimension.hpp"
#include "scale.hpp"
#include "string/unit_strings.hpp"

namespace mstd
{

    /**
     * @brief The quantity class
     *
     * @details This class represents a quantity with a value and a dimension
     * associated with it. It is used to represent physical quantities such as
     * length, mass, time, temperature, amount of substance, luminous intensity,
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     */
    template <typename T, typename Dim, typename Pack>
    class Quantity
    {
       public:
        using value_type = T;
        using dim_type   = Dim;
        using pack_type  = Pack;
        using eff_scale  = units::details::dim_scale_t<Dim, Pack>;

        constexpr Quantity() = default;

        constexpr explicit Quantity(const T value);

        template <typename OtherPack>
        constexpr Quantity(const Quantity<T, Dim, OtherPack>& other);

        constexpr T value() const;
        constexpr T si() const { return _si; }

        template <typename OtherPack>
        constexpr Quantity operator+(const Quantity<T, Dim, OtherPack>& other
        ) const;

        template <typename OtherPack>
        constexpr Quantity operator-(const Quantity<T, Dim, OtherPack>& other
        ) const;

        static constexpr Quantity from_si(const T si);

        static constexpr std::string get_unit_string();

        std::string to_string() const;
        std::string to_string(const size_t prec) const;
        std::string to_string(const size_t prec, const size_t width) const;
        std::string to_unit_string() const;
        std::string to_unit_string(const size_t prec) const;
        std::string to_unit_string(const size_t prec, const size_t width) const;

       private:
        T _si;
    };

    /**
     * @brief Constructs a Quantity from a value.
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     * @param value The value to construct the Quantity from.
     */
    template <typename T, typename Dim, typename Pack>
    constexpr Quantity<T, Dim, Pack>::Quantity(const T value)
        : _si(value * static_cast<T>(eff_scale::num) /
              static_cast<T>(eff_scale::den))
    {
    }

    /**
     * @brief Constructs a Quantity from another Quantity.
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     * @tparam OtherPack
     * @param other The Quantity to construct the new Quantity from.
     */
    template <typename T, typename Dim, typename Scale>
    template <typename OtherPack>
    constexpr Quantity<T, Dim, Scale>::Quantity(
        const Quantity<T, Dim, OtherPack>& other
    )
        : _si(other._si)
    {
    }

    /**
     * @brief Returns the value of the Quantity.
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     * @return constexpr T
     */
    template <typename T, typename Dim, typename Pack>
    constexpr T Quantity<T, Dim, Pack>::value() const
    {
        return _si * static_cast<T>(eff_scale::den) /
               static_cast<T>(eff_scale::num);
    }

    /**
     * @brief Adds two Quantities.
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     * @param other The Quantity to add.
     * @return constexpr Quantity
     */
    template <typename T, typename Dim, typename Pack>
    template <typename OtherPack>
    constexpr Quantity<T, Dim, Pack> Quantity<T, Dim, Pack>::operator+(
        const Quantity<T, Dim, OtherPack>& other
    ) const
    {
        return from_si(_si + other._si);
    }

    /**
     * @brief Subtracts two Quantities.
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     * @param other The Quantity to subtract.
     * @return constexpr Quantity
     */
    template <typename T, typename Dim, typename Pack>
    template <typename OtherPack>
    constexpr Quantity<T, Dim, Pack> Quantity<T, Dim, Pack>::operator-(
        const Quantity<T, Dim, OtherPack>& other
    ) const
    {
        return from_si(_si - other._si);
    }

    /**
     * @brief Constructs a Quantity from a n SI value.
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     * @param value The value to construct the Quantity from.
     */
    template <typename T, typename Dim, typename Pack>
    constexpr Quantity<T, Dim, Pack> from_si(const T si)
    {
        Quantity<T, Dim, Pack> q;
        q._si = si;
        return q;
    }

    /**
     * @brief Returns the unit of the Quantity.
     *
     * @tparam T
     * @tparam Dim
     * @tparam Pack
     * @return constexpr std::string
     */
    template <typename T, typename Dim, typename Pack>
    constexpr std::string Quantity<T, Dim, Pack>::get_unit_string()
    {
        return units::strings::get_unit_name<Dim, Pack>();
    }

    template <typename T, typename Dim, typename Pack>
    std::string Quantity<T, Dim, Pack>::to_string(
        const size_t precision,
        const size_t width
    ) const
    {
        std::ostringstream oss;

        oss << std::setw(width) << std::fixed << std::setprecision(precision)
            << value();

        return oss.str();
    }

    template <typename T, typename Dim, typename Pack>
    std::string Quantity<T, Dim, Pack>::to_string(const size_t precision) const
    {
        std::ostringstream oss;

        oss << std::fixed << std::setprecision(precision) << value();

        return oss.str();
    }

    template <typename T, typename Dim, typename Pack>
    std::string Quantity<T, Dim, Pack>::to_string() const
    {
        if constexpr (std::same_as<T, double> || std::same_as<T, float>)
            return to_string(6);
        else
            return std::to_string(value());
    }

    template <typename T, typename Dim, typename Pack>
    std::string Quantity<T, Dim, Pack>::to_unit_string(
        const size_t precision,
        const size_t width
    ) const
    {
        return to_string(precision, width) + " " + get_unit_string();
    }

    template <typename T, typename Dim, typename Pack>
    std::string Quantity<T, Dim, Pack>::to_unit_string(
        const size_t precision
    ) const
    {
        return to_string(precision) + " " + get_unit_string();
    }

    template <typename T, typename Dim, typename Pack>
    std::string Quantity<T, Dim, Pack>::to_unit_string() const
    {
        return to_string() + " " + get_unit_string();
    }

    /**
     * @brief friend operator* for Quantity
     *
     * @tparam T
     * @tparam Dim1
     * @tparam Pack1
     * @tparam Dim2
     * @tparam Pack2
     * @param lhs
     * @param rhs
     * @return constexpr auto
     */
    template <
        typename T,
        typename Dim1,
        typename Pack1,
        typename Dim2,
        typename Pack2>
    constexpr auto operator*(
        const Quantity<T, Dim1, Pack1>& lhs,
        const Quantity<T, Dim2, Pack2>& rhs
    )
    {
        using D = units::details::Dimension_add<Dim1, Dim2>;
        return Quantity<T, D, Pack1>::from_si(lhs.si() * rhs.si());
    }

    /**
     * @brief friend operator/ for Quantity
     *
     * @tparam T
     * @tparam Dim1
     * @tparam Pack1
     * @tparam Dim2
     * @tparam Pack2
     * @param lhs
     * @param rhs
     * @return constexpr auto
     */
    template <
        typename T,
        typename Dim1,
        typename Pack1,
        typename Dim2,
        typename Pack2>
    constexpr auto operator/(
        const Quantity<T, Dim1, Pack1>& lhs,
        const Quantity<T, Dim2, Pack2>& rhs
    )
    {
        using D = units::details::Dimension_sub<Dim1, Dim2>;
        return Quantity<T, D, Pack1>::from_si(lhs.si() / rhs.si());
    }

}   // namespace mstd

#endif   // __MSTD_UNITS_QUANTITY_HPP__