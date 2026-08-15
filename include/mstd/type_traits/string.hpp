#ifndef __MSTD__TYPE_TRAITS__STRING_HPP__
#define __MSTD__TYPE_TRAITS__STRING_HPP__

#include <concepts>
#include <string>

namespace mstd
{
    /**
     * @brief Concept checking whether Tag::toString(const T&) is a valid
     * expression that returns something convertible to std::string.
     *
     * @tparam Tag the tag type expected to provide a static toString member
     * @tparam T the value type passed to Tag::toString
     */
    template <typename Tag, typename T>
    concept HasToString = requires(const T &value) {
        { Tag::toString(value) } -> std::convertible_to<std::string>;
    };
}   // namespace mstd

#endif   // __MSTD__TYPE_TRAITS__STRING_HPP__
