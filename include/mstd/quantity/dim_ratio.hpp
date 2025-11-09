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

#ifndef __MSTD_DIM_RATIO_HPP__
#define __MSTD_DIM_RATIO_HPP__

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
    using default_si_ratio_pack = make_ratio_pack_t<SIDimIdMeta::size>;

    using default_si_power_ratio_pack =
        make_pow_ratio_pack_t<SIDimIdMeta::size, RatioPower<>>;

    /**
     * @brief default_extra_ratio_pack, which is a RatioPack of all Extra
     * dimensions
     */
    using default_extra_ratio_pack = make_ratio_pack_t<ExtraDimIdMeta::size>;

    using default_extra_power_ratio_pack =
        make_pow_ratio_pack_t<ExtraDimIdMeta::size, RatioPower<>>;

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
        RationalPowerPackType SiRatioPack    = default_si_power_ratio_pack,
        RationalPowerPackType ExtraRatioPack = default_extra_power_ratio_pack>
    struct DimRatio
    {
        static_assert(
            SiRatioPack::size == SIDimIdMeta::size,
            "DimRatio SiRatioPack size mismatch"
        );
        static_assert(
            ExtraRatioPack::size == ExtraDimIdMeta::size,
            "DimRatio ExtraRatioPack size mismatch"
        );
        // static assertion that all ratios in the packs are non-zero
        // (to avoid division by zero in ratio operations)
        static_assert(
            pack_utils<SiRatioPack>::count_non_zero() == SiRatioPack::size,
            "DimRatio SiRatioPack contains zero ratio(s)"
        );
        static_assert(
            pack_utils<ExtraRatioPack>::count_non_zero() ==
                ExtraRatioPack::size,
            "DimRatio ExtraRatioPack contains zero ratio(s)"
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

#endif   // __MSTD_DIM_RATIO_HPP__