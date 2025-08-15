#ifndef __MSTD_UNITS_TRAITS_HPP__
#define __MSTD_UNITS_TRAITS_HPP__

#include <type_traits>

#include "scale_tags.hpp"

namespace mstd
{
    // clang-format off
    struct pack_velocity_t{};
    struct pack_acceleration_t{};
    // clang-format on

    // clang-format off
    template<typename X> struct is_L : std::false_type {};
    template<typename R> struct is_L<L<R>> : std::true_type {};

    template<typename X> struct is_T : std::false_type {};
    template<typename R> struct is_T<T<R>> : std::true_type {};

    template<typename X> struct is_M : std::false_type {};
    template<typename R> struct is_M<M<R>> : std::true_type {};

    template<typename X> struct is_K : std::false_type {};
    template<typename R> struct is_K<K<R>> : std::true_type {};

    template<typename X> struct is_N : std::false_type {};
    template<typename R> struct is_N<N<R>> : std::true_type {};

    template<typename X> struct is_I : std::false_type {};
    template<typename R> struct is_I<I<R>> : std::true_type {};

    template<typename X> struct is_Cd : std::false_type {};
    template<typename R> struct is_Cd<Cd<R>> : std::true_type {};

    template<typename X> inline constexpr bool is_L_v = is_L<X>::value;
    template<typename X> inline constexpr bool is_T_v = is_T<X>::value;
    template<typename X> inline constexpr bool is_M_v = is_M<X>::value;
    template<typename X> inline constexpr bool is_K_v = is_K<X>::value;
    template<typename X> inline constexpr bool is_N_v = is_N<X>::value;
    template<typename X> inline constexpr bool is_I_v = is_I<X>::value;
    template<typename X> inline constexpr bool is_Cd_v = is_Cd<X>::value;
    // clang-format on

    template <typename T>
    inline constexpr bool is_value_type_v =
        std::is_arithmetic_v<std::remove_cv_t<std::remove_reference_t<T>>>;

    template <typename T>
    concept ValueType = is_value_type_v<T>;

    template <class X>
    concept LengthTag = is_L<X>::value;

    template <class X>
    concept TimeTag = is_T<X>::value;

    template <class X>
    concept MassTag = is_M<X>::value;

    template <class X>
    concept TemperatureTag = is_K<X>::value;

    template <class X>
    concept AmountOfSubstanceTag = is_N<X>::value;

    template <class X>
    concept LuminousIntensityTag = is_I<X>::value;

    template <class X>
    concept CurrentTag = is_Cd<X>::value;

    template <typename Pack, typename Kind>
    inline constexpr bool pack_is_v = std::is_same_v<typename Pack::kind, Kind>;

    template <typename Pack>
    concept VelocityPack = pack_is_v<Pack, pack_velocity_t>;

}   // namespace mstd

#endif