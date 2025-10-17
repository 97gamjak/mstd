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

#ifndef __MSTD_DIM_RATIO_DETAILS_HPP__
#define __MSTD_DIM_RATIO_DETAILS_HPP__

#include "dim_details.hpp"
#include "dim_ratio.hpp"
#include "mstd/pack.hpp"
#include "mstd/type_traits/ratio_traits.hpp"

namespace mstd
{
    namespace details
    {
        /***************************************
         *                                     *
         * Build ratio_dim from a single ratio *
         *                                     *
         ***************************************/

        /**
         * @brief struct to make a DimRatio from a dimension and a single ratio
         *
         * This struct is used to make a DimRatio from a dimension and a single
         * ratio. Important requirement is that the dimension is a simple base
         * dimension.
         *
         * @tparam Dim - has to be a simple base dimension
         * @tparam Ratio
         */
        template <details::SimpleDim Dim, RatioType Ratio>
        struct make_dim_ratio_single
        {
            static constexpr bool has_si = has_si_dim<Dim>();
            static constexpr bool has_ex = has_ex_dim<Dim>();

            // determine the index of the non-zero index in the dimension
            static constexpr size_t siIndex = Dim::si::non_zero_index();
            static constexpr size_t exIndex = Dim::ex::non_zero_index();

            static constexpr size_t si_size = Dim::si_size;
            static constexpr size_t ex_size = Dim::ex_size;

            // store the index and size of the non-zero dimension
            static constexpr size_t index        = has_si ? siIndex : exIndex;
            static constexpr size_t size         = has_si ? si_size : ex_size;
            static constexpr size_t default_size = has_si ? ex_size : si_size;

            // make the ratio packs
            using default_pack = make_default_ratio_pack_t<default_size>;
            using new_pack     = make_ratio_pack_single_t<Ratio, index, size>;

            // make the si and ex packs
            using si = std::conditional_t<has_si, new_pack, default_pack>;
            using ex = std::conditional_t<has_ex, new_pack, default_pack>;

            static_assert(si::size == Dim::si_size, "si size mismatch");
            static_assert(ex::size == Dim::ex_size, "ex size mismatch");

            // make the dim_ratio
            using type = DimRatio<si, ex>;
        };

        template <DimRatioType R, DimType D>
        struct dim_ratio_pow_impl
        {
            using type = DimRatio<
                ratio_pack_pow_t<typename R::si, typename D::si>,
                ratio_pack_pow_t<typename R::ex, typename D::ex>>;
        };

        template <DimRatioType R1, DimType D1, DimRatioType R2, DimType D2>
        struct dim_ratio_mul_impl
        {
            using _R1pow = dim_ratio_pow_impl<R1, D1>::type;
            using _R2pow = dim_ratio_pow_impl<R2, D2>::type;
            using _si1   = typename _R1pow::si;
            using _si2   = typename _R2pow::si;
            using _ex1   = typename _R1pow::ex;
            using _ex2   = typename _R2pow::ex;

            static_assert(_si1::size == D1::si_size, "si size mismatch");
            static_assert(_ex1::size == D1::ex_size, "ex size mismatch");
            static_assert(_si2::size == D2::si_size, "si size mismatch");
            static_assert(_ex2::size == D2::ex_size, "ex size mismatch");

            using _r1si = ratio_pack_mul_t<_si1, _si2>;
            using _r1ex = ratio_pack_mul_t<_ex1, _ex2>;

            using type = DimRatio<_r1si, _r1ex>;
        };

    }   // namespace details

}   // namespace mstd

#endif   // __MSTD_DIM_RATIO_DETAILS_HPP__