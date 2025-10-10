#ifndef __MSTD_TYPE_TRAITS_UNIT_HPP__
#define __MSTD_TYPE_TRAITS_UNIT_HPP__

#include <type_traits>

#include "mstd/tags/unit_tag.hpp"

namespace mstd
{
    template <typename T>
    constexpr bool is_unit_v = std::is_base_of_v<unit_tag, T>;

    template <typename T>
    concept UnitType = is_unit_v<T>;

}   // namespace mstd

#endif   // __MSTD_TYPE_TRAITS_UNIT_HPP__
