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

#ifndef __MSTD__QUANTITY__DIM_DETAILS_HPP__
#define __MSTD__QUANTITY__DIM_DETAILS_HPP__

#include "mstd/error.hpp"

MSTD_WARN_BUGGY_HEADER("mstd/quantity/dim_details.hpp")

#include "mstd/pack.hpp"

namespace mstd
{
    namespace details
    {
        /**
         * @brief Checks if a dimension has any physical units.
         *
         * @tparam Dim The dimension type to check.
         * @return true if the dimension has physical units, false otherwise.
         */
        template <typename Dim>
        inline constexpr size_t has_si_dim()
        {
            return Dim::si::num_vals_set() > 0;
        }

        /**
         * @brief Checks if a dimension has any extra dimensions.
         *
         * @tparam Dim The dimension type to check.
         * @return true if the dimension has extra dimensions, false otherwise.
         */
        template <typename Dim>
        inline constexpr size_t has_ex_dim()
        {
            return Dim::ex::num_vals_set() > 0;
        }

        /**
         * @brief Checks if a dimension is dimensionless (i.e., has no physical
         * units).
         *
         * @tparam D The dimension type to check.
         * @return true if the dimension is dimensionless, false otherwise.
         */
        template <typename D>
        inline constexpr bool is_dimensionless_v = []
        { return has_si_dim<D>() == 0 && has_ex_dim<D>() == 0; }();

        /**
         * @brief Checks if a dimension is a simple base dimension.
         *
         * @tparam Dim The dimension type to check.
         * @return true if the dimension is a simple base dimension, false
         * otherwise.
         */
        template <typename Dim>
        constexpr bool is_simple_base_dim()
        {
            return Dim::si::num_vals_set() + Dim::ex::num_vals_set() == 1;
        }

        /**
         * @brief Checks if a dimension is a simple base dimension.
         *
         * @tparam Dim The dimension type to check.
         * @return true if the dimension is a simple base dimension, false
         * otherwise.
         */
        template <typename Dim>
        concept SimpleDim = is_simple_base_dim<Dim>();

        /**
         * @brief Checks if a dimension is dimensionless (i.e., has no physical
         * units).
         *
         * @tparam Dim The dimension type to check.
         * @return true if the dimension is dimensionless, false otherwise.
         */
        template <typename Dim>
        concept ZeroDim = is_dimensionless_v<Dim>;

    }   // namespace details
}   // namespace mstd

#endif   // __MSTD__QUANTITY__DIM_DETAILS_HPP__