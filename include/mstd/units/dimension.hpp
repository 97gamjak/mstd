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

#ifndef __MSTD_UNITS_DIMENSION_HPP__
#define __MSTD_UNITS_DIMENSION_HPP__

#include <array>
#include <cstddef>

namespace mstd
{

    /**
     * @brief the dimension class
     *
     * @details This class represents the physical dimensions of a quantity,
     * including length, mass, time, temperature, amount of substance, luminous
     * intensity, and electric current.
     *
     * @tparam L
     * @tparam M
     * @tparam T
     * @tparam Th
     * @tparam N
     * @tparam I
     * @tparam Cd
     */
    template <int L, int M, int T, int Th, int N, int I, int Cd>
    struct dimension
    {
        static constexpr std::size_t           rank = 7;
        static constexpr std::array<int, rank> exponents{L, M, T, Th, N, I, Cd};

        static constexpr std::size_t length_index() { return 0; }
        static constexpr std::size_t mass_index() { return 1; }
        static constexpr std::size_t time_index() { return 2; }
        static constexpr std::size_t temp_index() { return 3; }
        static constexpr std::size_t amount_index() { return 4; }
        static constexpr std::size_t current_index() { return 5; }
        static constexpr std::size_t luminous_intensity_index() { return 6; }
    };

    namespace units::details
    {

        /**
         * @brief implements dimension addition
         *
         * @tparam D1
         * @tparam D2
         * @tparam Is
         * @return constexpr auto
         */
        template <typename D1, typename D2, std::size_t... Is>
        constexpr auto dimension_add_impl(std::index_sequence<Is...>)
        {
            return dimension<D1::exponents[Is] + D2::exponents[Is]...>{};
        }

        /**
         * @brief implements dimension subtraction
         *
         * @tparam D1
         * @tparam D2
         * @tparam Is
         * @return constexpr auto
         */
        template <typename D1, typename D2, std::size_t... Is>
        constexpr auto dimension_sub_impl(std::index_sequence<Is...>)
        {
            return dimension<D1::exponents[Is] - D2::exponents[Is]...>{};
        }

        /**
         * @brief type alias for dimension addition
         *
         * @tparam D1
         * @tparam D2
         * @return constexpr auto
         */
        template <typename D1, typename D2>
        using Dimension_add = decltype(dimension_add_impl<D1, D2>(
            std::make_index_sequence<D1::rank>{}
        ));

        /**
         * @brief type alias for dimension subtraction
         *
         * @tparam D1
         * @tparam D2
         * @return constexpr auto
         */
        template <typename D1, typename D2>
        using Dimension_sub = decltype(dimension_sub_impl<D1, D2>(
            std::make_index_sequence<D1::rank>{}
        ));

    }   // namespace units::details

}   // namespace mstd

namespace mstd
{
    using D_Length   = dimension<1, 0, 0, 0, 0, 0, 0>;
    using D_Mass     = dimension<0, 1, 0, 0, 0, 0, 0>;
    using D_Time     = dimension<0, 0, 1, 0, 0, 0, 0>;
    using D_Temp     = dimension<0, 0, 0, 1, 0, 0, 0>;
    using D_Amount   = dimension<0, 0, 0, 0, 1, 0, 0>;
    using D_I        = dimension<0, 0, 0, 0, 0, 1, 0>;
    using D_Cd       = dimension<0, 0, 0, 0, 0, 0, 1>;
    using D_UnitLess = dimension<0, 0, 0, 0, 0, 0, 0>;

    using D_Velocity     = units::details::Dimension_sub<D_Length, D_Time>;
    using D_Acceleration = units::details::Dimension_sub<D_Velocity, D_Time>;

}   // namespace mstd

#endif   // __MSTD_UNITS_DIMENSION_HPP__