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

#include <cstddef>

#include "enums.hpp"
#include "mstd/pack.hpp"

namespace mstd::units
{

    /***********************
     *                     *
     * The dimension class *
     *                     *
     ***********************/

    /**
     * @brief A class representing a physical dimension.
     *
     * @details A dimension is represented by two packs of integers:
     *          - The first pack represents the exponents of the 7 base SI
     * dimensions: length, mass, time, electric current, temperature, amount of
     * substance, luminous intensity.
     *          - The second pack represents the exponents of 4 additional
     * "extra" dimensions: angle, currency, information, count. Each pack is
     * implemented using the `details::integer_pack` class, which provides
     * compile-time storage and manipulation of integer sequences.
     * @tparam SIDimPack   A `details::integer_pack` representing the SI
     * dimension exponents.
     * @tparam ExtraDimPack A `details::integer_pack` representing the extra
     * dimension exponents.
     */
    template <class SIDimPack, class ExtraDimPack>
    struct dim
    {
        using si = SIDimPack;
        using ex = ExtraDimPack;

        static constexpr size_t si_size = SIDimPack::size;
        static constexpr size_t ex_size = ExtraDimPack::size;

        template <SIDimId ID>
        static constexpr int si_exp = SIDimPack::template get<ID>();

        template <ExtraDimId ID>
        static constexpr int ex_exp = ExtraDimPack::template get<ID>();
    };

    /*****************************
     *                           *
     * Dimension operations API  *
     *                           *
     *****************************/

    /**
     * @brief Adds two dimensions.
     *
     * @tparam D1 The first dimension type.
     * @tparam D2 The second dimension type.
     * @return The resulting dimension after adding the two dimensions.
     */
    template <class D1, class D2>
    using dim_mul_t =
        dim<pack::pack_add_t<typename D1::si, typename D2::si>,
            pack::pack_add_t<typename D1::ex, typename D2::ex>>;

    /**
     * @brief Subtracts one dimension from another.
     *
     * @tparam D1 The dimension type to subtract from.
     * @tparam D2 The dimension type to subtract.
     * @return The resulting dimension after subtraction.
     */
    template <class D1, class D2>
    using dim_div_t =
        dim<pack::pack_sub_t<typename D1::si, typename D2::si>,
            pack::pack_sub_t<typename D1::ex, typename D2::ex>>;

    /**
     * @brief Raises a dimension to an integer power.
     *
     * @tparam D The dimension type to raise to a power.
     * @tparam K The power to raise the dimension to.
     * @return The resulting dimension after raising to the power.
     */
    template <class D, int K>
    using dim_pow_t =
        dim<pack::pack_scale_t<typename D::si, K>,
            pack::pack_scale_t<typename D::ex, K>>;

    /*****************************
     *                           *
     * Dimension comparison API  *
     *                           *
     *****************************/

    template <class Dim>
    constexpr size_t has_si_dim()
    {
        return Dim::si::num_vals_set() > 0;
    }

    template <class Dim>
    constexpr size_t has_ex_dim()
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
    template <class D>
    inline constexpr bool is_dimensionless_v = []
    { return has_si_dim<D>() == 0 && has_ex_dim<D>() == 0; }();

    template <class Dim>
    constexpr bool is_simple_base_dim()
    {
        return Dim::si::num_vals_set() + Dim::ex::num_vals_set() == 1;
    }

    template <class Dim>
    concept SimpleDim = is_simple_base_dim<Dim>();

    template <class Dim>
    concept ZeroDim = is_dimensionless_v<Dim>;

}   // namespace mstd::units

#endif   // __MSTD_UNITS_DIMENSION_HPP__