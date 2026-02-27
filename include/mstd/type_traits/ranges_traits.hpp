#ifndef __MSTD__TYPE_TRAITS__RANGES_TRAITS_HPP__
#define __MSTD__TYPE_TRAITS__RANGES_TRAITS_HPP__

#include <ranges>

namespace mstd
{
    /**
     * @brief concept for ranges that can be joined into a string with a
     * delimiter
     *
     * @tparam Range
     */
    template <typename Range>
    concept joinable_range = std::ranges::input_range<Range> &&
                             std::convertible_to<
                                 std::ranges::range_reference_t<Range>,
                                 std::string_view>;
}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__RANGES_TRAITS_HPP__