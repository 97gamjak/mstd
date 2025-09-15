#ifndef __MSTD_RATIO_PACK_HPP__
#define __MSTD_RATIO_PACK_HPP__

#include <array>
#include <cstddef>
#include <ratio>
#include <tuple>

#include "mstd/ratio.hpp"

namespace mstd::pack
{
    /**
     * @brief A compile-time list of ratios.
     *
     * @tparam Rs The ratios to include in the ratio_pack.
     */
    template <class... Rs>
    struct ratio_pack
    {
        // store actual ratio values as long double so we can index them
        static constexpr std::array<long double, sizeof...(Rs)> vals{
            static_cast<long double>(Rs::num) / Rs::den...
        };

        static constexpr size_t size = sizeof...(Rs);

        template <auto I>
        using type_at = typename std::
            tuple_element<static_cast<size_t>(I), std::tuple<Rs...>>::type;

        /**
         * @brief Get the real ratio value at the specified index
         *
         * @tparam I
         * @return constexpr long double
         */
        template <auto I>
        static constexpr long double get()
        {
            return vals[static_cast<size_t>(I)];
        }
    };

    /***************
     *             *
     * Type traits *
     *             *
     ***************/

    namespace details
    {

        template <class>
        struct is_ratio_pack : std::false_type
        {
        };

        template <class... Rs>
        struct is_ratio_pack<ratio_pack<Rs...>> : std::true_type
        {
        };

    }   // namespace details

    template <class T>
    concept RatioPack = details::is_ratio_pack<T>::value;

    namespace details
    {

        template <
            class A,
            class B,
            template <class, class> class F,
            size_t... I>
        constexpr auto ratio_pack_zip_impl(std::index_sequence<I...>)
        {
            static_assert(A::size == B::size, "ratio_pack size mismatch");
            return ratio_pack<typename F<
                typename A::template type_at<I>,
                typename B::template type_at<I>>::type...>{};
        }

        template <class A, class B, template <class, class> class F>
        using ratio_pack_zip_t = decltype(ratio_pack_zip_impl<A, B, F>(
            std::make_index_sequence<A::size>{}
        ));

        // ---- apply power to every entry in a ratio_pack
        template <class Pack, int K, size_t... I>
        constexpr auto ratio_pack_pow_impl(std::index_sequence<I...>)
        {
            return ratio_pack<
                ratio::ratio_pow_t<typename Pack::template type_at<I>, K>...>{};
        }

        template <class Pack, size_t... I>
        consteval long double ratio_pack_value_impl(std::index_sequence<I...>)
        {
            auto r = static_cast<long double>(1);
            // fold over compile-time indices
            ((r *= Pack::template get<I>()), ...);
            return r;
        }

    }   // namespace details

    // helpers for multiply/divide
    template <RatioPack A, RatioPack B>
    using ratio_pack_mul_t =
        details::ratio_pack_zip_t<A, B, std::ratio_multiply>;

    template <RatioPack A, RatioPack B>
    using ratio_pack_div_t = details::ratio_pack_zip_t<A, B, std::ratio_divide>;

    template <class Pack, int K>
    using ratio_pack_pow_t = decltype(details::ratio_pack_pow_impl<Pack, K>(
        std::make_index_sequence<Pack::size>{}
    ));

    template <class Pack>
    inline constexpr long double ratio_pack_v =
        details::ratio_pack_value_impl<Pack>(
            std::make_index_sequence<Pack::size>{}
        );

    namespace details
    {

        // helper: always yields std::ratio<1>
        template <std::size_t>
        using default_ratio = std::ratio<1>;

        // main generator
        template <std::size_t N, typename Seq = std::make_index_sequence<N>>
        struct make_default_ratio_pack;

        template <std::size_t N, std::size_t... Is>
        struct make_default_ratio_pack<N, std::index_sequence<Is...>>
        {
            using type = ratio_pack<default_ratio<Is>...>;
        };

        template <size_t Idx, size_t... I>
        constexpr auto make_ratio_pack_at_impl(std::index_sequence<I...>)
        {
            return pack::ratio_pack<
                std::
                    conditional_t<I == Idx, std::ratio<1>, std::ratio<1>>...>{};
        }
        // overload taking the actual R
        template <class R, size_t Idx, size_t... I>
        constexpr auto make_ratio_pack_at_impl_R(std::index_sequence<I...>)
        {
            return pack::ratio_pack<
                std::conditional_t<I == Idx, R, std::ratio<1>>...>{};
        }

    }   // namespace details

    template <std::size_t N>
    using make_default_ratio_pack_t =
        typename details::make_default_ratio_pack<N>::type;

    template <class R, size_t Idx, size_t N>
    using make_ratio_pack_single_t =
        decltype(details::make_ratio_pack_at_impl_R<R, Idx>(
            std::make_index_sequence<N>{}
        ));

}   // namespace mstd::pack

#endif   // __MSTD_RATIO_PACK_HPP__