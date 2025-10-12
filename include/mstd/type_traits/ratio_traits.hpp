#ifndef __MSTD_RATIO_TRAITS_HPP__
#define __MSTD_RATIO_TRAITS_HPP__

namespace mstd
{

    /**
     * @brief Concept for std::ratio types.
     *
     * @details A ratio type must provide two static members `num` and `den`.
     */
    template <typename T>
    concept RatioType = requires {
        T::num;
        T::den;
    };

    template <typename T>
    constexpr bool is_ratio_v = RatioType<T>;

}   // namespace mstd

#endif   // __MSTD_RATIO_TRAITS_HPP__