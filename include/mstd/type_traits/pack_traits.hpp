#ifndef __MSTD_PACK_TRAITS_HPP__
#define __MSTD_PACK_TRAITS_HPP__

#include "ratio_traits.hpp"

namespace mstd
{
    /**
     * @brief Concept for integer pack types.
     *
     * @details An integer pack type must provide a static member `size` and
     * a member template `get<Index>()` returning the integer at position
     * `Index`.
     */
    template <typename T>
    concept IntegerPackType = requires {
        { T::size } -> std::convertible_to<std::size_t>;
        { T::template get<0>() } -> std::convertible_to<int>;
    };

    /**
     * @brief Concept for ratio pack types.
     *
     * @details A ratio pack type must provide a static member `size` and
     * a member template `get<Index>()` returning the `std::ratio` at
     * position `Index`.
     */
    template <typename T>
    concept RatioPackType = requires {
        { T::size } -> std::convertible_to<std::size_t>;
        is_ratio_v<decltype(T::template get<0>())>;
    };

}   // namespace mstd

#endif   // __MSTD_PACK_TRAITS_HPP__