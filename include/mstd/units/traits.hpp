#ifndef __MSTD_UNITS_TRAITS_HPP__
#define __MSTD_UNITS_TRAITS_HPP__

#include <type_traits>

namespace mstd
{
    namespace units::details
    {
        template <typename T>
        inline constexpr bool is_value_type_v =
            std::is_arithmetic_v<std::remove_cv_t<std::remove_reference_t<T>>>;

        template <typename T>
        concept ValueType = is_value_type_v<T>;

        // clang-format off
        template <typename Pack, typename Kind>
        inline constexpr bool pack_is_v =
            std::is_same_v<typename Pack::kind, Kind>;

    }   // namespace units::details

}   // namespace mstd

#endif