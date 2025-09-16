#ifndef __MSTD_DIM_RATIO_DETAILS_HPP__
#define __MSTD_DIM_RATIO_DETAILS_HPP__

#include "dimension.hpp"
#include "mstd/pack.hpp"

namespace mstd::units
{
    // forward declaration
    template <pack::RatioPack Si, pack::RatioPack Extra>
    struct dim_ratio;

    namespace details
    {
        /***************
         *             *
         * Type traits *
         *             *
         ***************/

        // clang-format off
        template <typename>
        struct is_dim_ratio : std::false_type{};

        template <typename pack1, typename pack2>
        struct is_dim_ratio<dim_ratio<pack1, pack2>> : std::true_type{};
        // clang-format on

        /**
         * @brief concept for dim_ratio types
         *
         * @tparam T
         */
        template <typename T>
        concept DimRatio = is_dim_ratio<T>::value;

        /**
         * @brief function to check if a type is a dim_ratio
         *
         * @tparam T
         */
        template <typename T>
        inline constexpr bool is_dim_ratio_v = is_dim_ratio<T>::value;

        /***************************************
         *                                     *
         * Build ratio_dim from a single ratio *
         *                                     *
         ***************************************/

        /**
         * @brief struct to make a dim_ratio from a dimension and a single ratio
         *
         * This struct is used to make a dim_ratio from a dimension and a single
         * ratio. Important requirement is that the dimension is a simple base
         * dimension.
         *
         * @tparam Dim - has to be a simple base dimension
         * @tparam Ratio
         */
        template <SimpleDim Dim, ratio::StdRatio Ratio>
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
            using default_pack = pack::make_default_ratio_pack_t<default_size>;
            using new_pack = pack::make_ratio_pack_single_t<Ratio, index, size>;

            // make the si and ex packs
            using si = std::conditional_t<has_si, new_pack, default_pack>;
            using ex = std::conditional_t<has_ex, new_pack, default_pack>;

            static_assert(si::size == Dim::si_size, "si size mismatch");
            static_assert(ex::size == Dim::ex_size, "ex size mismatch");

            // make the dim_ratio
            using type = dim_ratio<si, ex>;
        };

    }   // namespace details

}   // namespace mstd::units

#endif   // __MSTD_DIM_RATIO_DETAILS_HPP__