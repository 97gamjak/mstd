#ifndef __MSTD_DIM_RATIO_HPP__
#define __MSTD_DIM_RATIO_HPP__

#include "dim_ratio_details.hpp"
#include "enums.hpp"
#include "mstd/pack.hpp"

namespace mstd::units
{
    /***********************
     *                     *
     * Default ratio packs *
     *                     *
     ***********************/

    /**
     * @brief default_si_ratio_pack, which is a ratio_pack of all SI dimensions
     */
    using default_si_ratio_pack =
        pack::make_default_ratio_pack_t<SIDimIdMeta::size>;

    /**
     * @brief default_extra_ratio_pack, which is a ratio_pack of all Extra
     * dimensions
     */
    using default_extra_ratio_pack =
        pack::make_default_ratio_pack_t<ExtraDimIdMeta::size>;

    /***********************
     *                     *
     * The dim_ratio class *
     *                     *
     ***********************/

    /**
     * @brief The dim_ratio class
     *
     * This class is used to store the ratio for each dimension
     * of a quantity. It contains two compile time lists of ratios.
     * The first list is for the SI dimensions and the second list is for the
     * Extra dimensions.
     * @tparam SiRatioPack    A `pack::ratio_pack` representing the SI
     * dimension ratios.
     * @tparam ExtraRatioPack A `pack::ratio_pack` representing the Extra
     * dimension ratios.
     *
     */
    template <
        pack::RatioPack SiRatioPack    = default_si_ratio_pack,
        pack::RatioPack ExtraRatioPack = default_extra_ratio_pack>
    struct dim_ratio
    {
        // putting static asserts instead of requires to make forward
        // declaration easier to handle and modify
        static_assert(
            SiRatioPack::size == SIDimIdMeta::size,
            "dim_ratio SiRatioPack size mismatch"
        );
        static_assert(
            ExtraRatioPack::size == ExtraDimIdMeta::size,
            "dim_ratio ExtraRatioPack size mismatch"
        );

        using si = SiRatioPack;
        using ex = ExtraRatioPack;

        /**
         * @brief get the ratio for a specific dimension
         *
         * @tparam ID The dimension ID for example SIDimId::Length
         */
        template <SIDimId ID>
        using si_ratio =
            typename SiRatioPack::template type_at<static_cast<size_t>(ID)>;

        /**
         * @brief get the ratio for a specific dimension
         *
         * @tparam ID The dimension ID for example ExtraDimId::Currency
         */
        template <ExtraDimId ID>
        using ex_ratio =
            typename ExtraRatioPack::template type_at<static_cast<size_t>(ID)>;
    };

    /*****************************
     *                           *
     * dim_ratio type operations *
     *                           *
     *****************************/

    /**
     * @brief dim_ratio multiplication
     *
     * @tparam Ratio1
     * @tparam Ratio2
     */
    template <details::DimRatio Ratio1, details::DimRatio Ratio2>
    using dim_ratio_mul_t = dim_ratio<
        pack::ratio_pack_mul_t<typename Ratio1::si, typename Ratio2::si>,
        pack::ratio_pack_mul_t<typename Ratio1::ex, typename Ratio2::ex>>;

    /**
     * @brief dim_ratio division
     *
     * @tparam R1
     * @tparam R2
     */
    template <details::DimRatio Ratio1, details::DimRatio Ratio2>
    using dim_ratio_div_t = dim_ratio<
        pack::ratio_pack_div_t<typename Ratio1::si, typename Ratio2::si>,
        pack::ratio_pack_div_t<typename Ratio1::ex, typename Ratio2::ex>>;

    /**
     * @brief dim_ratio power
     *
     * @tparam DimRatio
     * @tparam Exp
     */
    template <details::DimRatio Ratio, int Exp>
    using dim_ratio_pow_t = dim_ratio<
        pack::ratio_pack_pow_t<typename Ratio::si, Exp>,
        pack::ratio_pack_pow_t<typename Ratio::ex, Exp>>;

    /***************************************
     *                                     *
     * Build ratio_dim from a single ratio *
     *                                     *
     ***************************************/

    /**
     * @brief make a dim_ratio from a dimension and a single ratio
     *
     * @tparam D
     * @tparam R
     */
    template <SimpleDim Dim, ratio::StdRatio Ratio>
    using make_dim_ratio_single_t =
        typename details::make_dim_ratio_single<Dim, Ratio>::type;

}   // namespace mstd::units

#endif   // __MSTD_DIM_RATIO_HPP__