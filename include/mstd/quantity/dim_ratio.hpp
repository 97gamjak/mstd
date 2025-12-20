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

#ifndef __MSTD__QUANTITY__DIM_RATIO_HPP__
#define __MSTD__QUANTITY__DIM_RATIO_HPP__

#include "mstd/error.hpp"

MSTD_WARN_BUGGY_HEADER("mstd/quantity/dim_ratio.hpp")

#include "enums.hpp"
#include "mstd/pack.hpp"

namespace mstd
{
    /***********************
     *                     *
     * Default ratio packs *
     *                     *
     ***********************/

    /**
     * @brief default_si_ratio_pack, which is a RatioPack of all SI dimensions
     */
    using default_si_ratio_pack = make_default_ratio_pack_t<SIDimIdMeta::size>;

    /**
     * @brief default_extra_ratio_pack, which is a RatioPack of all Extra
     * dimensions
     */
    using default_extra_ratio_pack =
        make_default_ratio_pack_t<ExtraDimIdMeta::size>;

    /***********************
     *                     *
     * The DimRatio class  *
     *                     *
     ***********************/

    /**
     * @brief The DimRatio class
     *
     * This class is used to store the ratio for each dimension
     * of a quantity. It contains two compile time lists of ratios.
     * The first list is for the SI dimensions and the second list is for the
     * Extra dimensions.
     * @tparam SiRatioPack    A `RatioPack` representing the SI
     * dimension ratios.
     * @tparam ExtraRatioPack A `RatioPack` representing the Extra
     * dimension ratios.
     *
     */
    template <
        RatioPackType SiRatioPack    = default_si_ratio_pack,
        RatioPackType ExtraRatioPack = default_extra_ratio_pack>
    struct DimRatio
    {
        // putting static asserts instead of requires to make forward
        // declaration easier to handle and modify
        static_assert(
            SiRatioPack::size == SIDimIdMeta::size,
            "DimRatio SiRatioPack size mismatch"
        );
        static_assert(
            ExtraRatioPack::size == ExtraDimIdMeta::size,
            "DimRatio ExtraRatioPack size mismatch"
        );

        using si = SiRatioPack;
        using ex = ExtraRatioPack;

        /**
         * @brief get the ratio for a specific dimension
         *
         * @tparam ID The dimension ID for example SIDimId::Length
         * @return the ratio at the given index
         */
        template <SIDimId ID>
        using si_ratio =
            typename SiRatioPack::template type_at<static_cast<size_t>(ID)>;

        /**
         * @brief get the ratio for a specific dimension
         *
         * @tparam ID The dimension ID for example ExtraDimId::Currency
         * @return the ratio at the given index
         */
        template <ExtraDimId ID>
        using ex_ratio =
            typename ExtraRatioPack::template type_at<static_cast<size_t>(ID)>;
    };

}   // namespace mstd

#endif   // __MSTD__QUANTITY__DIM_RATIO_HPP__