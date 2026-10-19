#ifndef __MSTD__TYPE_TRAITS__COMPARISON_HPP__
#define __MSTD__TYPE_TRAITS__COMPARISON_HPP__

#include <compare>

namespace mstd
{
    /**
     * @brief Variable template to check if types are three-way comparable.
     *
     * Evaluates to `true` if all types in the parameter pack satisfy the
     * `std::three_way_comparable` concept, otherwise evaluates to `false`.
     */
    template <typename... Ts>
    inline constexpr bool three_way_comparable =
        (sizeof...(Ts) > 0) && (std::three_way_comparable<Ts> && ...);

}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__COMPARISON_HPP__
