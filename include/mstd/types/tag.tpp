#ifndef __MSTD__TYPES__TAG_TPP__
#define __MSTD__TYPES__TAG_TPP__

#include <algorithm>
#include <cstddef>

#include "tag.hpp"

namespace mstd
{
    /**
     * @brief Construct a new Tag object
     *
     * @param str The string literal to initialize the tag with
     */
    template <size_t N>
    constexpr Tag<N>::Tag(const char (&str)[N])
    {
        std::copy_n(str, N, value);
    }

    /**
     * @brief Returns a string view of the tag's value
     *
     * @return constexpr std::string_view
     */
    template <size_t N>
    constexpr std::string_view Tag<N>::view() const
    {
        return {value, N - 1};
    }

    /**
     * @brief Converts the tag's value to a std::string
     *
     * @return std::string
     */
    template <size_t N>
    std::string Tag<N>::to_string() const
    {
        return std::string(value, N - 1);
    }
}   // namespace mstd

#endif   // __MSTD__TYPES__TAG_TPP__
