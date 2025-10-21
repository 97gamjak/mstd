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

#ifndef __MSTD_INTEGER_PACK_OPERATIONS_HPP__
#define __MSTD_INTEGER_PACK_OPERATIONS_HPP__

#include "integer_pack.hpp"
#include "integer_pack_details.hpp"

namespace mstd
{
    /*************************
     *                       *
     * Convenience aliases   *
     *                       *
     *************************/
    /** Add two packs element-wise. */
    template <class A, class B>
    using pack_add_t = details::pack_zip_t<A, B, add_fn>;
    /** Subtract two packs element-wise. */
    template <class A, class B>
    using pack_sub_t = details::pack_zip_t<A, B, sub_fn>;
    /** Scale a pack by a compile-time integer K. */
    template <class P, int K>
    using pack_scale_t = details::pack_map_t<P, scale_fn<K>>;

    /*********************
     *                   *
     * Factory aliases   *
     *                   *
     *********************/
    /** Create an `IntegerPack` of size N filled with zeros. */
    template <std::size_t N, int value = 0>
    using make_integer_pack_t =
        typename details::make_integer_pack<N, value>::type;

    /** Create an `IntegerPack` of size N with 0s except index Ix set to I. */
    template <std::size_t N, std::size_t Ix, int I = 1>
    using make_single_integer_pack_t =
        typename details::make_single_integer_pack<N, Ix, I>::type;

}   // namespace mstd

#endif   // __MSTD_INTEGER_PACK_OPERATIONS_HPP__